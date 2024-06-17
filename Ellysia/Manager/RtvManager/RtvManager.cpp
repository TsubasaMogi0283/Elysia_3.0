#include "RtvManager.h"
#include "WindowsSetup.h"

RtvManager* RtvManager::GetInstance(){
    static RtvManager instance;
    return &instance;
}


ComPtr<ID3D12Resource> RtvManager::CreateRenderTextureResource(
	uint32_t width, uint32_t height, DXGI_FORMAT format, const Vector4 clearColor) {

	D3D12_RESOURCE_DESC resourceDesc{};
	//Textureの幅
	resourceDesc.Width = width;
	//Textureの高さ
	resourceDesc.Height = height;
	//mipmapの数
	resourceDesc.MipLevels = 1;
	//奥行 or 配列Textureの配列数
	resourceDesc.DepthOrArraySize = 1;
	//利用可能なフォーマット
	resourceDesc.Format = format;
	//サンプリングカウント。1固定
	resourceDesc.SampleDesc.Count = 1;
	//2次元
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	//RenderTargetとして利用可能にする
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

	//resourceDescとclearValueにあるFormatはしっかり統一させてね


	//利用するHeapの設定
	D3D12_HEAP_PROPERTIES heapProperties{};
	//VRAM上に作る
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;




	//クリア設定
	D3D12_CLEAR_VALUE clearValue{};
	clearValue.Format = format;
	clearValue.Color[0] = clearColor.x;
	clearValue.Color[1] = clearColor.y;
	clearValue.Color[2] = clearColor.z;
	clearValue.Color[3] = clearColor.w;

	//D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE

	//Resourceの作成
	ComPtr<ID3D12Resource> resource = nullptr;
	HRESULT hr = DirectXSetup::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapProperties,					//Heapの設定 
		D3D12_HEAP_FLAG_NONE,				//Heapの特殊な設定。特になし。
		&resourceDesc,						//Resourceの設定
		D3D12_RESOURCE_STATE_RENDER_TARGET,	//これから描画することを前提としたTextureなのでRenderTargetとして使うことから始める
		&clearValue,						//Clear最適値。ClearRenderTargetをこの色でClearするようにする。最適化されているので高速！
		IID_PPV_ARGS(&resource));			//作成するResourceポインタへのポインタ
	assert(SUCCEEDED(hr));


	return resource;
}


uint32_t RtvManager::Allocate(){
	//上限だったらasset
	assert(index_ < RTV_DESCRIPTOR_SIZE_);

	//return する番号を一旦記録しておく
	int index = index_;
	
	//次のために番号を1進める
	index_++;

	//上で記録した番号をreturn
	return index;


}

void RtvManager::GenarateRenderTargetView(ComPtr<ID3D12Resource> resource,uint32_t handle){
	//RTVの設定
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;				//出力結果をSRGBに変換して書き込む
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	
	//縦横を取得
	
	
	//0の時だけ少し違うのでそれ専用の
	if (handle == 0) {
		//ディスクリプタの先頭を取得する
		D3D12_CPU_DESCRIPTOR_HANDLE rtvStartHandle;
		rtvStartHandle = m_rtvDescriptorHeap_->GetCPUDescriptorHandleForHeapStart();
		rtvHandles_[handle] = rtvStartHandle;
	}
	else {
		//ハンドルを計算
		rtvHandles_[handle].ptr = rtvHandles_[handle - 1].ptr + DirectXSetup::GetInstance()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);;

	}

	
	
	//RTVの作成(本体)
	DirectXSetup::GetInstance()->GetDevice()->CreateRenderTargetView(
		resource.Get(), &rtvDesc, rtvHandles_[handle]);


}




void RtvManager::Initialize(){
	//Resourceに対して作業を行うにはViewが必要
	//Viewは作業方法
	
	//DescriptorHeapを作る
	m_rtvDescriptorHeap_ = DirectXSetup::GetInstance()->GenarateDescriptorHeap(
		D3D12_DESCRIPTOR_HEAP_TYPE_RTV, RTV_DESCRIPTOR_SIZE_, false);


	//RTVの設定
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
	SwapChain swapChain = DirectXSetup::GetInstance()->GetSwapChain();
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;				//出力結果をSRGBに変換して書き込む
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;			//2dテクスチャとして書き込む
	

	////5つ目
	////DepthBasedOutline用
	//depthBasedOutlineResource_ = CreateRenderTextureResource(
	//	width, height, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, OUT_LINE_CLEAR_VALUE);
	//
	//
	//rtvHandles_[4].ptr = rtvHandles_[3].ptr + DirectXSetup::GetInstance()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	//
	//DirectXSetup::GetInstance()->GetDevice()->CreateRenderTargetView(
	//	depthBasedOutlineResource_.Get(), &rtvDesc, rtvHandles_[4]);
	//
	//
	//
	//
	//
	////RadialBlur
	//const Vector4 RADIAL_BLUR_CLEAR_VALUE = { 0.1f,0.1f,0.7f,1.0f };
	//radialBlurTextureResource_ = CreateRenderTextureResource(
	//	width, height, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, RADIAL_BLUR_CLEAR_VALUE);
	//
	//rtvHandles_[5].ptr = rtvHandles_[4].ptr + DirectXSetup::GetInstance()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	//
	//DirectXSetup::GetInstance()->GetDevice()->CreateRenderTargetView(
	//	radialBlurTextureResource_.Get(), &rtvDesc, rtvHandles_[5]);
	//
	//
	//
	////Dissove
	//const Vector4 DISSOLVE_CLEAR_VALUE = { 0.1f,0.1f,0.7f,1.0f };
	//dissolveTextureResource_ = CreateRenderTextureResource(
	//	width, height, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, DISSOLVE_CLEAR_VALUE);
	//
	//rtvHandles_[6].ptr = rtvHandles_[5].ptr + DirectXSetup::GetInstance()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	//
	//DirectXSetup::GetInstance()->GetDevice()->CreateRenderTargetView(
	//	dissolveTextureResource_.Get(), &rtvDesc, rtvHandles_[6]);
	//
	//
	////RandomEffect
	//const Vector4 RANDOM_EFFECT_VALUE = { 0.1f,0.1f,0.7f,1.0f };
	//randomEffectTextureResource_ = CreateRenderTextureResource(
	//	width, height, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, RANDOM_EFFECT_VALUE);
	//
	//rtvHandles_[7].ptr = rtvHandles_[6].ptr + DirectXSetup::GetInstance()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	//
	//DirectXSetup::GetInstance()->GetDevice()->CreateRenderTargetView(
	//	randomEffectTextureResource_.Get(), &rtvDesc, rtvHandles_[7]);




	rtvDesc_ = rtvDesc;
	//rtvStartHandle_=rtvStartHandle;
}



