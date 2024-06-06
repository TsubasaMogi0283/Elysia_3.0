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
	//ディスクリプタの先頭を取得する
	D3D12_CPU_DESCRIPTOR_HANDLE rtvStartHandle;
	rtvStartHandle = m_rtvDescriptorHeap_->GetCPUDescriptorHandleForHeapStart();

	//SwapChain用
	rtvHandles_[0] = rtvStartHandle;
	DirectXSetup::GetInstance()->GetDevice()->CreateRenderTargetView(DirectXSetup::GetInstance()->GetSwapChain().m_pResource[0].Get(), &rtvDesc, rtvHandles_[0]);
	//２つ目のディスクリプタハンドルを得る(自力で)
	rtvHandles_[1].ptr = rtvHandles_[0].ptr + DirectXSetup::GetInstance()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	//２つ目を作る
	DirectXSetup::GetInstance()->GetDevice()->CreateRenderTargetView(DirectXSetup::GetInstance()->GetSwapChain().m_pResource[1].Get(), &rtvDesc, rtvHandles_[1]);



	
	
	//3つ目
	const Vector4 RENDER_TARGET_CLEAR_VALUE = { 1.0f,0.0f,0.0f,1.0f };//今回は赤
	//縦横を取得
	uint32_t width = (WindowsSetup::GetInstance()->GetClientWidth());
	uint32_t height = (WindowsSetup::GetInstance()->GetClientHeight());
	
	renderTextureResource = CreateRenderTextureResource(
		width, height, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, RENDER_TARGET_CLEAR_VALUE);
	
	
	rtvHandles_[2].ptr = rtvHandles_[1].ptr + DirectXSetup::GetInstance()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	
	DirectXSetup::GetInstance()->GetDevice()->CreateRenderTargetView(
		renderTextureResource.Get(), &rtvDesc, rtvHandles_[2]);



	//関数でまとめて簡単に生成できるようにしたい
	//4つ目
	//LuminanceBasedOutline用
	const Vector4 OUT_LINE_CLEAR_VALUE = { 0.1f,0.1f,0.7f,1.0f };

	outLineTextureResource = CreateRenderTextureResource(
		width, height, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, OUT_LINE_CLEAR_VALUE);
	

	rtvHandles_[3].ptr = rtvHandles_[2].ptr + DirectXSetup::GetInstance()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	DirectXSetup::GetInstance()->GetDevice()->CreateRenderTargetView(
		outLineTextureResource.Get(), &rtvDesc, rtvHandles_[3]);

	//5つ目
	//DepthBasedOutline用
	depthBasedOutlineResource_ = CreateRenderTextureResource(
		width, height, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, OUT_LINE_CLEAR_VALUE);


	rtvHandles_[4].ptr = rtvHandles_[3].ptr + DirectXSetup::GetInstance()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	DirectXSetup::GetInstance()->GetDevice()->CreateRenderTargetView(
		depthBasedOutlineResource_.Get(), &rtvDesc, rtvHandles_[4]);







	rtvDesc_ = rtvDesc;
	rtvStartHandle_=rtvStartHandle;
}

