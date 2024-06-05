#include "TextureManager.h"
#include "SrvManager.h"



static uint32_t descriptorSizeSRV_ = 0u;
static uint32_t descriptorSizeRTV_ = 0u;
static uint32_t descriptorSizeDSV_ = 0u;
static uint32_t textureIndex;


static DirectX::ScratchImage mipImages_[TextureManager::TEXTURE_MAX_AMOUNT_];
static D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc[TextureManager::TEXTURE_MAX_AMOUNT_];

//コンストラクタ
TextureManager::TextureManager() {

}

TextureManager* TextureManager::GetInstance() {
	
	static TextureManager instance;
	return &instance;
}




const D3D12_RESOURCE_DESC TextureManager::GetResourceDesc(uint32_t textureHandle) {
	//テクスチャの情報を取得
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc = textureInformation_[textureHandle].resource_->GetDesc();

	return resourceDesc;
}


//初期化
void TextureManager::Initilalize() {
	//this->directXSetup_ = DirectXSetup::GetInstance();
	//COMの初期化
	//COM...ComponentObjectModel、Microsoftの提唱する設計技術の１つ
	//		DirectX12も簡略化されたCOM(Nano-COM)という設計で作られている
	
	//COMを使用して開発されたソフトウェア部品をCOMコンポーネントと呼ぶ
	//Textureを読むにあたって、COMコンポーネントの１つを利用する
	CoInitializeEx(0, COINIT_MULTITHREADED);

	descriptorSizeSRV_ =  DirectXSetup::GetInstance()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	descriptorSizeRTV_ =  DirectXSetup::GetInstance()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	descriptorSizeDSV_ =  DirectXSetup::GetInstance()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	textureIndex = 0;
}




//統合させた関数
uint32_t TextureManager::LoadTexture(const std::string& filePath) {

	//一度読み込んだものはその値を返す
	//新規は勿論読み込みをする
	for (int i = 0; i < TEXTURE_MAX_AMOUNT_; i++) {
		//同じテクスチャがあった場合そのテクスチャハンドルを返す
		if (TextureManager::GetInstance()->textureInformation_[i].name_ == filePath) {
			return TextureManager::GetInstance()->textureInformation_[i].handle_;
		}
	}



	//読み込むたびにインデックスが増やし重複を防ごう
	//同じ画像しか貼れなかったのはこれが原因
	textureIndex= SrvManager::GetInstance()->Allocate();



	//読み込んだデータを配列に保存
	//テクスチャの名前
	TextureManager::GetInstance()->textureInformation_[textureIndex].name_ = filePath;
	//テクスチャハンドル
	TextureManager::GetInstance()->textureInformation_[textureIndex].handle_ = textureIndex;

	//Textureを読んで転送する
	mipImages_[textureIndex] = LoadTextureData(filePath);

	const DirectX::TexMetadata& metadata = mipImages_[textureIndex].GetMetadata();

	TextureManager::GetInstance()->textureInformation_[textureIndex].resource_ = CreateTextureResource(metadata);
	UploadTextureData(TextureManager::GetInstance()->textureInformation_[textureIndex].resource_.Get(), mipImages_[textureIndex]);


	//ShaderResourceView
	//今のDescriptorHeapには
	//0...ImGui
	//1...uvChecker
	//2...monsterBall
	//3...NULL
	//.
	//.
	//このような感じで入っている
	//後ろのindexに対応させる
	

	//SRVの確保
	//0番目はImGuiが使っているからダメだった
	TextureManager::GetInstance()->textureInformation_[textureIndex].handle_ = textureIndex;

	//SRVの生成
	SrvManager::GetInstance()->CreateSRVForTexture2D(
		TextureManager::GetInstance()->textureInformation_[textureIndex].handle_,
		TextureManager::GetInstance()->textureInformation_[textureIndex].resource_.Get(),
		metadata.format, UINT(metadata.mipLevels));


	return textureIndex;
}
	


#pragma region テクスチャの読み込み
//1.TextureデータそのものをCPUで読み込む
//2.DirectX12のTextureResourceを作る
//3.CPUで書き込む用にUploadHeapのResourceを作る(IntermediateResource)
//4.3に対してCPUでデータを書き込む
//5.CommandListに3を2に転送するコマンドを積む
//6.CommandQueueを使って実行する
//7.6の実行完了を待つ


#pragma region 上のLoadTextureにまとめた
//Textureを読み込むためのLoad関数
//1.TextureデータそのものをCPUで読み込む
DirectX::ScratchImage TextureManager::LoadTextureData(const std::string& filePath) {
	
	//テクスチャファイルを読んでプログラムで扱えるようにする
	DirectX::ScratchImage image{};
	std::wstring filePathW = ConvertString(filePath);
	//ここで問題
	HRESULT hr = DirectX::LoadFromWICFile(filePathW.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
	assert(SUCCEEDED(hr));
	
	//ミップマップの作成
	//ミップマップ...元画像より小さなテクスチャ群
	DirectX::ScratchImage mipImages{};
	hr = DirectX::GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::TEX_FILTER_SRGB, 0, mipImages);
	assert(SUCCEEDED(hr));

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
	
	HRESULT hr =  DirectXSetup::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapProperties,					//Heapの設定
		D3D12_HEAP_FLAG_NONE,				//Heapの特殊な設定
		&resourceDesc,						//Resourceの設定
		D3D12_RESOURCE_STATE_GENERIC_READ,	//初回のResourceState。データの転送を受け入れられるようにする
		nullptr,							//Clear最適値。使わないのでnullptr
		IID_PPV_ARGS(&resource));			//作成するResourceポインタへのポインタ
	assert(SUCCEEDED(hr));

	return resource;


}

//3.TextureResourceに1で読んだデータを転送する
//書き換え
void TextureManager::UploadTextureData(
	ComPtr<ID3D12Resource> texture, 
	const DirectX::ScratchImage& mipImages) {

	//Meta情報を取得
	const DirectX::TexMetadata& metadata = mipImages.GetMetadata();
	//全MipMapについて
	for (size_t mipLevel = 0; mipLevel < metadata.mipLevels; ++mipLevel) {
		//MipMapLevelを指定して各Imageを取得
		const DirectX::Image* img = mipImages.GetImage(mipLevel, 0, 0);
		//Textureに転送
		HRESULT hr = texture->WriteToSubresource(
			UINT(mipLevel),
			nullptr,				//全領域へコピー
			img->pixels,			//元データアドレス
			UINT(img->rowPitch),	//1ラインサイズ
			UINT(img->slicePitch)	//1枚サイズ
		);

		assert(SUCCEEDED(hr));
	}



}



#pragma endregion



#pragma endregion


void TextureManager::GraphicsCommand(uint32_t rootParameter, uint32_t texHandle) {
	SrvManager::GetInstance()->SetGraphicsRootDescriptorTable(rootParameter, texHandle);
}

void TextureManager::Release() {
	
	
	//ゲーム終了時にはCOMの終了処理を行っておく
	CoUninitialize();
}


//コンストラクタ
TextureManager::~TextureManager() {

}


