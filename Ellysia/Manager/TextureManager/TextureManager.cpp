#include "TextureManager.h"
#include "SrvManager.h"

#include "d3dx12.h"
#include <vector>

uint32_t TextureManager::index_ = 0u;


TextureManager* TextureManager::GetInstance() {

	static TextureManager instance;
	return &instance;
}




const D3D12_RESOURCE_DESC TextureManager::GetResourceDesc(const uint32_t& textureHandle) {
	//テクスチャの情報を取得
	// handleからfilePathを取得
	auto it = handleToFilePathMap_.find(textureHandle);
	if (it != handleToFilePathMap_.end()) {
		const std::string& filePath = it->second;
		return textureInformation_[filePath].resource_->GetDesc();
	}

	// エラーハンドリング: 見つからなかった場合の空のリソース記述子を返す
	return D3D12_RESOURCE_DESC{};
}




//統合させた関数
uint32_t TextureManager::LoadTexture(const std::string& filePath) {

	TextureManager* textureManager = TextureManager::GetInstance();

	//一度読み込んだものはその値を返す
	//新規は勿論読み込みをする
	auto it = TextureManager::GetInstance()->textureInformation_.find(filePath);
	if (it != TextureManager::GetInstance()->textureInformation_.end()) {
		return it->second.handle_;
	}



	//読み込むたびにインデックスを増やし重複を防ごう
	index_ = SrvManager::GetInstance()->Allocate();



	//読み込んだデータを保存
	TextureInformation textureInfo;
	textureInfo.handle_ = index_;
	textureInfo.name_ = filePath;

	//テクスチャハンドル
	textureInfo.handle_ = index_;

	//テクスチャの名前
	textureInfo.name_ = filePath;


	//Textureを読んで転送する
	textureInfo.mipImages_ = LoadTextureData(filePath);

	const DirectX::TexMetadata& metadata = textureInfo.mipImages_.GetMetadata();

	textureInfo.resource_ = CreateTextureResource(metadata);
	textureInfo.internegiateResource_ = TransferTextureData(
		textureInfo.resource_.Get(), textureInfo.mipImages_).Get();




	//SRVの生成
	SrvManager::GetInstance()->CreateSRVForTexture2D(
		textureInfo.handle_,
		textureInfo.resource_.Get(),
		metadata.format, UINT(metadata.mipLevels), metadata.IsCubemap());


	// 読み込んだデータをmapに保存
	textureManager->GetTextureInformation()[filePath] = std::move(textureInfo);

	TextureManager::GetInstance()->handleToFilePathMap_[index_] = filePath; 

	return textureInfo.handle_;
}



#pragma region テクスチャの読み込み
//1.TextureデータそのものをCPUで読み込む
//2.DirectX12のTextureResourceを作る
//3.CPUで書き込む用にUploadHeapのResourceを作る(IntermediateResource)
//4.3に対してCPUでデータを書き込む
//5.CommandListに3を2に転送するコマンドを積む
//6.CommandQueueを使って実行する
//7.6の実行完了を待つ



//CubeMap...箱を構成する6毎のTextureをひとまとめにしたTextureのこと
//さいころみたいなもの


//DSSについて
//DSS...DirectDrawSurface
//png,jpegだとCubemapを使えないがDSSだと出来る!!
//GPUに圧縮したまま送ることが出来るので処理速度が速い




#pragma region 上のLoadTextureにまとめた
//Textureを読み込むためのLoad関数
//1.TextureデータそのものをCPUで読み込む

DirectX::ScratchImage TextureManager::LoadTextureData(const std::string& filePath) {

	HRESULT hResult = {};

	//テクスチャファイルを読んでプログラムで扱えるようにする
	DirectX::ScratchImage image{};
	std::wstring filePathW = ConvertString::ToWString(filePath);
	//dssファイルの場合
	if (filePathW.ends_with(L".dds")) {
		hResult = DirectX::LoadFromDDSFile(filePathW.c_str(), DirectX::DDS_FLAGS_NONE, nullptr, image);
		assert(SUCCEEDED(hResult));
	}
	//その他のpngやjpegなど
	else {
		hResult = DirectX::LoadFromWICFile(filePathW.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
		assert(SUCCEEDED(hResult));

	}

	//ミップマップの作成
	//ミップマップ...元画像より小さなテクスチャ群
	DirectX::ScratchImage mipImages{};
	//圧縮フォーマットかどうかを調べる
	if (DirectX::IsCompressed(image.GetMetadata().format)) {
		//圧縮フォーマットならそのまま使う
		mipImages = std::move(image);
	}
	else {
		hResult = DirectX::GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::TEX_FILTER_SRGB, 4, mipImages);
		assert(SUCCEEDED(hResult));

	}


	//ミップマップ月のデータを返す
	return mipImages;
}

//2.DirectX12のTextureResourceを作る

ComPtr<ID3D12Resource> TextureManager::CreateTextureResource(const DirectX::TexMetadata& metadata) {
	ComPtr<ID3D12Resource> resource = nullptr;

	//1.metadataを基にResourceの設定
	D3D12_RESOURCE_DESC resourceDesc{};
	//Textureの幅
	resourceDesc.Width = UINT(metadata.width);
	//Textureの高さ
	resourceDesc.Height = UINT(metadata.height);
	//mipmapの数
	resourceDesc.MipLevels = UINT16(metadata.mipLevels);
	//奥行き or 配列Textureの配列数
	resourceDesc.DepthOrArraySize = UINT16(metadata.arraySize);
	//TextureのFormat
	resourceDesc.Format = metadata.format;
	//サンプリングカウント
	resourceDesc.SampleDesc.Count = 1;
	//Textureの次元数。普段使っているのは2次元
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION(metadata.dimension);

	//2.利用するHeapの設定
	//利用するHeapの設定。非常に特殊な運用。02_04exで一般的なケース版がある
	D3D12_HEAP_PROPERTIES heapProperties{};
	//細かい設定を行う
	heapProperties.Type = D3D12_HEAP_TYPE_CUSTOM;
	//WriteBackポリシーでCPUアクセス可能
	heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	//プロセッサの近くに配置
	heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;




	//3.Resourceを生成する

	HRESULT hResult = DirectXSetup::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapProperties,					//Heapの設定
		D3D12_HEAP_FLAG_NONE,				//Heapの特殊な設定
		&resourceDesc,						//Resourceの設定
		D3D12_RESOURCE_STATE_COPY_DEST,		//初回のResourceState。データの転送を受け入れられるようにする
		nullptr,							//Clear最適値。使わないのでnullptr
		IID_PPV_ARGS(&resource));			//作成するResourceポインタへのポインタ
	assert(SUCCEEDED(hResult));

	return resource;


}

//3.TextureResourceに1で読んだデータを転送する
//書き換え

[[nodiscard]]
ComPtr<ID3D12Resource> TextureManager::TransferTextureData(ComPtr<ID3D12Resource> texture, const DirectX::ScratchImage& mipImages) {

	std::vector<D3D12_SUBRESOURCE_DATA> subresources;
	DirectX::PrepareUpload(DirectXSetup::GetInstance()->GetDevice().Get(), mipImages.GetImages(), mipImages.GetImageCount(), mipImages.GetMetadata(), subresources);
	uint64_t intermidiateSize = GetRequiredIntermediateSize(texture.Get(), 0, UINT(subresources.size()));
	ComPtr<ID3D12Resource> intermediateSizeResource = DirectXSetup::GetInstance()->CreateBufferResource(intermidiateSize);
	UpdateSubresources(DirectXSetup::GetInstance()->GetCommandList().Get(), texture.Get(), intermediateSizeResource.Get(), 0, 0, UINT(subresources.size()), subresources.data());

	//Textureへの転送後は利用できるよう、D312_RESOURCE_STATE_COPY_DESTから
	//D3D12_RESOURCE_STATE_GENERIC_READへResourceStateを変更する
	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = texture.Get();
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;
	DirectXSetup::GetInstance()->GetCommandList()->ResourceBarrier(1, &barrier);


	return intermediateSizeResource;
}



#pragma endregion



#pragma endregion


void TextureManager::GraphicsCommand(const uint32_t& rootParameter, const uint32_t& textureHandle) {
	SrvManager::GetInstance()->SetGraphicsRootDescriptorTable(rootParameter, textureHandle);
}

