#include "DirectXSetup.h"
#include <thread>
#include <d3dx12.h>


#include <PipelineManager.h>
#include <SrvManager.h>
#include "../RtvManager/RtvManager.h"

//インスタンス
DirectXSetup* DirectXSetup::GetInstance() {
	//関数内static変数として宣言する
	static DirectXSetup instance;
	return &instance;
}




//DescriptorHeapの作成関数
ComPtr<ID3D12DescriptorHeap> DirectXSetup::GenarateDescriptorHeap(
		D3D12_DESCRIPTOR_HEAP_TYPE heapType,
		UINT numDescriptors, bool shaderVisible) {

	ComPtr<ID3D12DescriptorHeap> descriptorHeap= nullptr;
	D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc{};
	descriptorHeapDesc.Type = heapType;
	descriptorHeapDesc.NumDescriptors = numDescriptors;
	descriptorHeapDesc.Flags = shaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	HRESULT hr = DirectXSetup::GetInstance()->m_device_->CreateDescriptorHeap(&descriptorHeapDesc, IID_PPV_ARGS(&descriptorHeap));
	assert(SUCCEEDED(hr));
	return descriptorHeap;

}

//Resource作成の関数化
ComPtr<ID3D12Resource> DirectXSetup::CreateBufferResource(size_t sizeInBytes) {
	//void返り値も忘れずに
	ComPtr<ID3D12Resource> resource = nullptr;
	
	////VertexResourceを生成
	//頂点リソース用のヒープを設定
	//関数用
	D3D12_HEAP_PROPERTIES uploadHeapProperties_{};
	
	uploadHeapProperties_.Type = D3D12_HEAP_TYPE_UPLOAD;

	//頂点リソースの設定
	D3D12_RESOURCE_DESC vertexResourceDesc_{};
	//バッファリソース。テクスチャの場合はまた別の設定をする
	vertexResourceDesc_.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	vertexResourceDesc_.Width = sizeInBytes;
	//バッファの場合はこれらは1にする決まり
	vertexResourceDesc_.Height = 1;
	vertexResourceDesc_.DepthOrArraySize = 1;
	vertexResourceDesc_.MipLevels = 1;
	vertexResourceDesc_.SampleDesc.Count = 1;

	//バッファの場合はこれにする決まり
	vertexResourceDesc_.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//実際に頂点リソースを作る
	//ID3D12Resource* vertexResource_ = nullptr;
	
	//次はここで問題
	//hrは調査用
	HRESULT hr;
	hr = DirectXSetup::GetInstance()->GetDevice()->CreateCommittedResource(
		&uploadHeapProperties_,
		D3D12_HEAP_FLAG_NONE,
		&vertexResourceDesc_,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr, IID_PPV_ARGS(&resource));
	assert(SUCCEEDED(hr));

	return resource;
}

ComPtr<ID3D12Resource> DirectXSetup::CreateDepthStencilTextureResource(const int32_t width,const int32_t height) {
	D3D12_RESOURCE_DESC resourceDesc{};
	//Textureの幅
	resourceDesc.Width = width;
	//Textureの高さ
	resourceDesc.Height = height;
	//mipmapの数
	resourceDesc.MipLevels = 1;
	//奥行 or 配列Textureの配列数
	resourceDesc.DepthOrArraySize = 1;
	//DepthStencilとして利用可能なフォーマット
	resourceDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	//サンプリングカウント。1固定
	resourceDesc.SampleDesc.Count = 1;
	//2次元
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	//DepthStencilとして使う通知
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	//利用するHeapの設定
	D3D12_HEAP_PROPERTIES heapProperties{};
	//VRAM上に作る
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;

	//深度値のクリア設定
	D3D12_CLEAR_VALUE depthClearValue{};
	//1.0f(最大値)でクリア
	depthClearValue.DepthStencil.Depth = 1.0f;
	//フォーマット。Resourceと合わせる
	depthClearValue.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	//Resourceの作成
	ComPtr<ID3D12Resource> resource = nullptr;
	HRESULT hr = DirectXSetup::GetInstance()->m_device_->CreateCommittedResource(
		&heapProperties,					//Heapの設定 
		D3D12_HEAP_FLAG_NONE,				//Heapの特殊な設定。特になし。
		&resourceDesc,						//Resourceの設定
		D3D12_RESOURCE_STATE_DEPTH_WRITE,	//深度値を書き込む状態にしておく
		&depthClearValue,					//Clear最適値
		IID_PPV_ARGS(&resource));			//作成するResourceポインタへのポインタ
	assert(SUCCEEDED(hr));


	return resource;

}




#pragma region Initializeの所で使う関数

void DirectXSetup::GenerateDXGIFactory() {
	//DXGIファクトリーの生成
#ifdef _DEBUG
	ComPtr<ID3D12Debug1> debugController;
	//ID3D12Debug1* debugController = nullptr;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
		//デバッグレイヤーを有効化する
		debugController->EnableDebugLayer();
		//さらにGPU側でもチェックを行うようにする
		debugController->SetEnableGPUBasedValidation(TRUE);

	}


#endif 
	
}

void DirectXSetup::SelectAdapter() {
	//仕様するアダプタ用の変数、最初にnullptrを入れておく

	ComPtr<IDXGIFactory7> dxgiFactory;
	//関数が成功したかSUCCEEDEDでマクロで判定できる
	HRESULT hr = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));
	//初期でエラーが発生した場合どうにもできないのでassert
	assert(SUCCEEDED(hr));
	DirectXSetup::GetInstance()->m_dxgiFactory_ = dxgiFactory;

	ComPtr<IDXGIAdapter4> useAdapter = nullptr;

	//良い順でアダプタを頼む
	for (UINT i = 0; dxgiFactory->EnumAdapterByGpuPreference(i,
		DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&useAdapter)) !=
		DXGI_ERROR_NOT_FOUND; i++) {

		//アダプターの情報を取得する
		DXGI_ADAPTER_DESC3 adapterDesc{};
		hr = useAdapter->GetDesc3(&adapterDesc);
		assert(SUCCEEDED(hr));

		//ソフトウェアアダプタでなければ採用
		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE)) {
			//採用したアダプタの情報をログに出力.(wstring)
			Log(ConvertString(std::format(L"Use Adapter:{}\n", adapterDesc.Description)));
			break;
		}
		//ソフトウェアアダプタだった場合無視
		useAdapter = nullptr;


	}
	//適切なアダプタが見つからなかったので起動できない
	assert(useAdapter != nullptr);
	
	//更新したデータをメンバ変数にまた保存
	DirectXSetup::GetInstance()->m_dxgiFactory_ = dxgiFactory;
	DirectXSetup::GetInstance()->m_useAdapter_ = useAdapter;



}

void DirectXSetup::GenerateD3D12Device() {
	//機能レベルとログ出力用の文字
	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_12_2,
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0
	};
	

	const char* featureLevelStrings[] = { "12.2","12.1","12.0" };
	//高い順に生成出来るか試していく
	for (size_t i = 0; i < _countof(featureLevels); ++i) {
		//採用したアダプターでデバイスが生成
		HRESULT hr = {};
		hr = D3D12CreateDevice(
			DirectXSetup::GetInstance()->m_useAdapter_.Get(), 
			featureLevels[i], 
			IID_PPV_ARGS(&DirectXSetup::GetInstance()->m_device_));
		//指定した機能レベルでデバイスが生成できたか確認
		if (SUCCEEDED(hr)) {
			//生成できたのでログ出力を行ってループを抜ける
			Log(std::format("FeatureLevel : {}\n", featureLevelStrings[i]));
			break;
		}
	}

	//デバイスの生成が上手くいかなかったので起動できない
	assert(DirectXSetup::GetInstance()->m_device_ != nullptr);
	Log("Complete create D3D12Device!!!\n");

}

void DirectXSetup::StopErrorWarning() {

#ifdef _DEBUG
	

		////エラー・警告、即ち停止
	
	ComPtr<ID3D12InfoQueue> infoQueue;
	if (SUCCEEDED(DirectXSetup::GetInstance()->m_device_->QueryInterface(IID_PPV_ARGS(&infoQueue)))) {
		//ヤバいエラー時に止まる
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
		//エラー時に止まる
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
		//警告時に止まる
		////全ての情報を出す
		//以下をコメントアウト
		//大丈夫だった場合元に戻してあげる
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);
		

		////エラーと警告の抑制
		//Windowsの不具合だと解消できない
		//その時に停止させないよう特定のエラーや警告を無視するしかない

		//抑制するメッセージのID 		
		D3D12_MESSAGE_ID denyIds[] = {
			//Windows11でのDXGデバッグれーやーとDX12デバッグレイヤーの相互作用バグによるエラーメッセージ
			D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_COMMAND_LIST_TYPE
		};

		//抑制する
		D3D12_MESSAGE_SEVERITY severities[] = { D3D12_MESSAGE_SEVERITY_INFO };
		D3D12_INFO_QUEUE_FILTER filter{};

		filter.DenyList.NumIDs = _countof(denyIds);
		filter.DenyList.pIDList = denyIds;
		filter.DenyList.NumSeverities = _countof(severities);
		filter.DenyList.pSeverityList = severities;
		//指定したメッセージの表示を抑制する
		infoQueue->PushStorageFilter(&filter);
		
		

	}


	
#endif 

}

void DirectXSetup::GenerateCommand() {
	////GPUに作業させよう
	//コマンドキューを生成する
	HRESULT hr = {};
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc_{};
	ComPtr<ID3D12CommandQueue> commandQueue = nullptr;
	hr = DirectXSetup::GetInstance()->m_device_->CreateCommandQueue(&commandQueueDesc_, IID_PPV_ARGS(&commandQueue));
	//コマンドキューの生成が上手くいかなかったので起動できない
	assert(SUCCEEDED(hr));

	//コマンドアロケータを生成する
	ComPtr<ID3D12CommandAllocator> commandAllocator = nullptr;
	hr = DirectXSetup::GetInstance()->m_device_->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator));
	//コマンドアロケータの生成が上手くいかなかったので起動できない
	assert(SUCCEEDED(hr));

	//コマンドリストを生成する
	ComPtr<ID3D12GraphicsCommandList> commandList = nullptr;
	hr = DirectXSetup::GetInstance()->m_device_->CreateCommandList(
		0, 
		D3D12_COMMAND_LIST_TYPE_DIRECT, 
		commandAllocator.Get(),
		nullptr, 
		IID_PPV_ARGS(&commandList));

	//コマンドリストの生成が上手くいかなかったので起動できない
	assert(SUCCEEDED(hr));

	//ローカルに入れた値をメンバ変数に保存しよう
	DirectXSetup::GetInstance()->m_commandQueue_ = commandQueue;
	DirectXSetup::GetInstance()->m_commandAllocator_ = commandAllocator;
	DirectXSetup::GetInstance()->m_commandList_ = commandList;
}

void DirectXSetup::GenerateSwapChain() {
	
	//60fpsそのまま映すと大変なので2枚用意して
	//描画(フロントバッファ)と表示(バックバッファ、プライマリバッファ)に分ける。
	//このことをダブルバッファリングという。
	HWND hwnd = WindowsSetup::GetInstance()->GetHwnd();
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	swapChainDesc.Width = WindowsSetup::GetInstance()->GetClientWidth();		//画面の幅。ウィンドウのクライアント領域を同じものにしておく
	swapChainDesc.Height = WindowsSetup::GetInstance()->GetClientHeight();	//画面の高さ。ウィンドウのクライアント領域を同じものにしておく
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;					//色の形式
	swapChainDesc.SampleDesc.Count = 1;								//マルチサンプルしない
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;		//描画のターゲットとして利用する
	swapChainDesc.BufferCount = 2;										//ダブルバッファ
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;			//モニタにうつしたら中身を破棄


	//コマンドキュー、ウィンドウハンドル、設定を渡して生成する
	//ComPtr<ID3D12CommandQueue>  commandQueue = DirectXSetup::GetInstance()->m_commandQueue_;
	
	HRESULT hr = {};
	hr = DirectXSetup::GetInstance()->m_dxgiFactory_->CreateSwapChainForHwnd(
		DirectXSetup::GetInstance()->m_commandQueue_.Get(),
		hwnd, 
		&swapChainDesc, 
		nullptr, 
		nullptr, 
		reinterpret_cast<IDXGISwapChain1**>(&DirectXSetup::GetInstance()->swapChain));
	assert(SUCCEEDED(hr));
	
	DirectXSetup::GetInstance()->swapChain.swapChainDesc = swapChainDesc;

}

void DirectXSetup::MakeDescriptorHeap() {
	
	
	




	ComPtr<ID3D12DescriptorHeap> dsvDescriptorHeap_ = nullptr;
	ComPtr<ID3D12Resource> depthStencilResource_ = nullptr;
	//ImGuiを使うにはSRV用のDescriptorが必要となる
	



	depthStencilResource_ = CreateDepthStencilTextureResource( 
		WindowsSetup::GetInstance()->GetClientWidth(),
		WindowsSetup::GetInstance()->GetClientHeight());

	dsvDescriptorHeap_ = GenarateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 1, false);



	//DSVの設定
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
	//Format 基本的にはResourceに合わせる
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	//2DTexture
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	//DSVHeapの先頭にDSVを作る
	DirectXSetup::GetInstance()->m_device_->CreateDepthStencilView(
		depthStencilResource_.Get(), 
		&dsvDesc, 
		dsvDescriptorHeap_->GetCPUDescriptorHandleForHeapStart());

	DirectXSetup::GetInstance()->m_dsvDescriptorHeap_ = dsvDescriptorHeap_;
	DirectXSetup::GetInstance()->m_depthStencilResource_ = depthStencilResource_;
}

void DirectXSetup::PullResourcesFromSwapChain() {
	HRESULT hr = {};
	hr = DirectXSetup::GetInstance()->swapChain.m_pSwapChain->GetBuffer(0, IID_PPV_ARGS(& DirectXSetup::GetInstance()->swapChain.m_pResource[0]));
	//上手く取得できなければ起動できない
	assert(SUCCEEDED(hr));
	hr = DirectXSetup::GetInstance()->swapChain.m_pSwapChain->GetBuffer(1, IID_PPV_ARGS(& DirectXSetup::GetInstance()->swapChain.m_pResource[1]));
	assert(SUCCEEDED(hr));



}

void DirectXSetup::SetRTV() {
	//RTVの設定
	//D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
	//SwapChain swapChain = DirectXSetup::GetInstance()->swapChain;
	//
	//
	//rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;				//出力結果をSRGBに変換して書き込む
	//rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;			//2dテクスチャとして書き込む
	////ディスクリプタの先頭を取得する
	//D3D12_CPU_DESCRIPTOR_HANDLE rtvStartHandle;
	//rtvStartHandle = DirectXSetup::GetInstance()->m_rtvDescriptorHeap_->GetCPUDescriptorHandleForHeapStart();


	//RTVを２つ作るのでディスクリプタを２つ用意
	//PostEffectでもう一つ必要になったので3つに増やす
	//まず1つ目を作る。１つ目は最初の所に作る。作る場所をこちらで指定してあげる必要がある
	//DirectXSetup::GetInstance()->rtvHandles_[0] = rtvStartHandle;
	//DirectXSetup::GetInstance()->m_device_->CreateRenderTargetView(DirectXSetup::GetInstance()->swapChain.m_pResource[0].Get(), &rtvDesc, DirectXSetup::GetInstance()->rtvHandles_[0]);
	////２つ目のディスクリプタハンドルを得る(自力で)
	//DirectXSetup::GetInstance()->rtvHandles_[1].ptr = DirectXSetup::GetInstance()->rtvHandles_[0].ptr + DirectXSetup::GetInstance()->m_device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	////２つ目を作る
	//DirectXSetup::GetInstance()->m_device_->CreateRenderTargetView(DirectXSetup::GetInstance()->swapChain.m_pResource[1].Get(), &rtvDesc, DirectXSetup::GetInstance()->rtvHandles_[1]);


	//上の2つはSwapChain用

	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = DirectXSetup::GetInstance()->m_dsvDescriptorHeap_->GetCPUDescriptorHandleForHeapStart();
	DirectXSetup::GetInstance()->dsvHandle_ = dsvHandle;

	////3つ目
	//const Vector4 RENDER_TARGET_CLEAR_VALUE = { 1.0f,0.0f,0.0f,1.0f };//今回は赤
	////縦横を取得
	//uint32_t width = (WindowsSetup::GetInstance()->GetClientWidth());
	//uint32_t height = (WindowsSetup::GetInstance()->GetClientHeight());
	//
	//DirectXSetup::GetInstance()->renderTextureResource = CreateRenderTextureResource(
	//	width, height, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, RENDER_TARGET_CLEAR_VALUE);
	//
	//
	//DirectXSetup::GetInstance()->rtvHandles_[2].ptr = DirectXSetup::GetInstance()->rtvHandles_[1].ptr + DirectXSetup::GetInstance()->m_device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	//
	//DirectXSetup::GetInstance()->m_device_->CreateRenderTargetView(
	//	DirectXSetup::GetInstance()->renderTextureResource.Get(), &rtvDesc, DirectXSetup::GetInstance()->rtvHandles_[2]);

	//SrvManager::GetInstance()-> CreateSRVForRenderTexture(DirectXSetup::GetInstance()->renderTextureResource.Get());


	////FenceとEvent
	//Fence・・・CPUとGPUの同期を取るために利用するオブジェクト。
	//			 GPUで値を書き込み、CPUで値を読み取ったりWindowsにメッセージ(Event)を送ったりできる
	//			 理想を実現するためのもの
	//Event・・・Windowsへのメッセージなどのこと
	//初期位置0でフェンスを作る
	//EventはWindowsのものである
	HRESULT hr = {};
	uint64_t fenceValue = 0;
	ComPtr<ID3D12Fence> fence = nullptr;
	hr = DirectXSetup::GetInstance()->m_device_->CreateFence(fenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
	assert(SUCCEEDED(hr));

	//FenceのSignalを待つためのイベントを作成する
	HANDLE fenceEvent = nullptr;
	fenceEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	assert(fenceEvent != nullptr);

	//DirectXSetup::GetInstance()->rtvDesc_ = rtvDesc;
	//DirectXSetup::GetInstance()->rtvStartHandle_=rtvStartHandle;
	DirectXSetup::GetInstance()->fenceValue_ = fenceValue;
	DirectXSetup::GetInstance()->fenceEvent_ = fenceEvent;
	DirectXSetup::GetInstance()->m_fence_ = fence;


}



void DirectXSetup::GenarateViewport() {
	////ViewportとScissor
	//ビューポート
	
	D3D12_VIEWPORT viewport{};
	//クライアント領域のサイズと一緒にして画面全体に表示
	viewport.Width = float(WindowsSetup::GetInstance()->GetClientWidth());
	viewport.Height = float(WindowsSetup::GetInstance()->GetClientHeight());
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	
	DirectXSetup::GetInstance()->viewport_ = viewport;

}

void DirectXSetup::GenerateScissor() {
	//シザー矩形 
	D3D12_RECT scissorRect{};
	//基本的にビューポートと同じ矩形が構成されるようにする
	scissorRect.left = 0;
	scissorRect.right = WindowsSetup::GetInstance()->GetClientWidth();
	scissorRect.top = 0;
	scissorRect.bottom = WindowsSetup::GetInstance()->GetClientHeight();


	DirectXSetup::GetInstance()->scissorRect_ = scissorRect;

}


//FPS固定初期化
void DirectXSetup::InitializeFPS() {
	//現在時間を記録する
	//初期化前の時間を記録
	DirectXSetup::GetInstance()->reference_ = std::chrono::steady_clock::now();

	//std::chrono::steady_clock...逆行しないタイマー
}

#pragma endregion

void DirectXSetup::FirstInitialize() {
	//出力ウィンドウへの文字出力
	OutputDebugStringA("Hello,DirectX!\n");

	InitializeFPS();

	//DXGIFactoryを生成
	GenerateDXGIFactory();
	//最適なアダプターを選択
	SelectAdapter();


	//デバイスを生成
	GenerateD3D12Device();

	////エラー・警告、即ち停止
	StopErrorWarning();

	

	////GPUに作業させよう
	//コマンドリストを作ろう
	GenerateCommand();


	//スワップチェーンを生成する
	GenerateSwapChain();



	//Resource...DirectX12が管理しているGPU上のメモリであり、このデータのこと
	//View...Resourceに対してどのような処理を行うのか手順をまとめたもの

	//Descriptor...view(作業方法)の情報を格納している場所
	//DescriptorHeap...Descriptorを束ねたもの


	//流れ
	//1.DescriptorHeapを生成する
	//2.swapChainからResourceを引っ張ってくる
	//3.引っ張ってきたResourceに対してDescriptor上にRTVを作る

	////DescriptorHeap(RTV用)を生成する
	MakeDescriptorHeap();

	//スワップチェーンを引っ張ってくる
	PullResourcesFromSwapChain();

}

void DirectXSetup::SecondInitialize(){

	//RenderTargetViewの設定
	SetRTV();


	//DXCの初期化
	////ShaderCompile
	//ShaderはHLSLによって記述されているが、GPUが解釈できる形ではない
	//一度DXIL(DirectX Intermediate Language)というドライバ用の形式に変換され、
	//ドライバがGPU用のバイナリに変更しやっと実行されるよ。手間だね。
	// 
	// DXC(DirectX Shader Compiler)がHLSLからDXILにするCompilerである
	//


	//ビューポートの生成
	GenarateViewport();

	//シザーを生成
	GenerateScissor();

}



	//FPS固定更新
void DirectXSetup::UpdateFPS() {
	//1/60秒ピッタリの時間
	//1フレームの時間
	const std::chrono::microseconds MIN_TIME(uint64_t(1000000.0f / 60.0f));
	
	//1/60秒よりわずかに短い時間
	const std::chrono::microseconds MIN_CHECK_TIME(uint64_t(1000000.0f / 65.0f));



	//現在時間を取得する
	//VSync待ちが完了した時点での時間を取得する
	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();

	//前回記録からの経過時間を取得する
	//現在時間から前回の時間を引くことで前回からの経過時間を取得する
	std::chrono::microseconds elapsed = std::chrono::duration_cast<std::chrono::microseconds>(now - DirectXSetup::GetInstance()->reference_);

	//前回から1/60秒経つまで待機する
	//1/60秒(よりわずかに短い時間)経っていない場合
	if (elapsed < MIN_CHECK_TIME) {
		//1/60秒経過するまで微小なスリープを繰り返す
		while (std::chrono::steady_clock::now() - DirectXSetup::GetInstance()->reference_ < MIN_TIME) {
			//1マイクロ秒スリープ
			std::this_thread::sleep_for(std::chrono::microseconds(1));
		}
	}

	//次のフレームの計算に使うため、待機完了後の時間を記録しておく
	//現在の時間を記録する
	DirectXSetup::GetInstance()->reference_ = std::chrono::steady_clock::now();




}



void DirectXSetup::StartDraw() {

	////コマンドをキックする
	//コマンドを積む・・・CommandListに処理を追加していくこと
	//キックする・・・CommandQueueCommandListを渡してGPUの実行を開始すること
	//画面をクリアするためのコマンドを積み、キックし、メインループを完成させる



	//処理の内容
	//1.BackBufferを決定する
	//2.書き込む作業(画面のクリア)をしたいのでRTVを設定する
	//3.画面のクリアを行う
	//4.CommandListを閉じる
	//5.CommandListの実行(キック)
	//6.画面のスワップ(BackBufferとFrontBufferを入れ替える)
	//7.次のフレーム用にCommandListを再準備

	

	///////
	////コマンドを積みこんで確定させる
	//LoadCommand()
	//これから書き込むバックバッファのインデックスを取得
	backBufferIndex_ = DirectXSetup::GetInstance()->swapChain.m_pSwapChain->GetCurrentBackBufferIndex();

	////TransitionBarrierを張るコード
	//現在のResourceStateを設定する必要がある → ResorceがどんなStateなのかを追跡する必要がある
	//追跡する仕組みはStateTrackingという
	//
	//TransitionBarrierの設定
	//今回のバリアはTransition
	//D3D12_RESOURCE_STATE_PRESENT...見せるモード
	//D3D12_RESOURCE_STATE_RENDER_TARGET...RTV使う用

	barrier_.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	//Noneにする
	barrier_.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	//バリアを張る対象のリソース。現在のバックバッファに対して行う
	barrier_.Transition.pResource = DirectXSetup::GetInstance()->swapChain.m_pResource[backBufferIndex_].Get();
	//遷移前(現在)のResourceState
	barrier_.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	//遷移後のResourceState
	barrier_.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	//TransitionBarrierを張る
	DirectXSetup::GetInstance()->GetCommandList()->ResourceBarrier(1, &barrier_);

	//描画先のRTVとDSVを設定する
	//描画先のRTVを設定する
	DirectXSetup::GetInstance()->GetCommandList()->OMSetRenderTargets(1, &RtvManager::GetInstance()->GetRtvHandle(backBufferIndex_), false, &DirectXSetup::GetInstance()->dsvHandle_);
	//指定した色で画面全体をクリアする
	float clearColor[] = { 0.1f,0.25f,0.5f,1.0f };	//青っぽい色
	DirectXSetup::GetInstance()->GetCommandList()->ClearRenderTargetView(RtvManager::GetInstance()->GetRtvHandle(backBufferIndex_), clearColor, 0, nullptr);

	DirectXSetup::GetInstance()->GetCommandList()->RSSetViewports(1, &viewport_);
	DirectXSetup::GetInstance()->GetCommandList()->RSSetScissorRects(1, &scissorRect_);


}





void DirectXSetup::EndDraw() {
	////画面表示出来るようにする
	//ここがflameの最後
	//画面に描く処理は「全て終わり」、画面に映すので、状態を遷移
	//今回はRenderTargetからPresentにする
	

	//遷移前(現在)のResourceState
	barrier_.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	//遷移後のResourceState
	barrier_.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	//TransitionBarrierを張る
	DirectXSetup::GetInstance()->GetCommandList()->ResourceBarrier(1, &barrier_);


	//コマンドリストの内容を確定させる。全てのコマンドを積んでからCloseすること
	HRESULT hr = {};
	hr = DirectXSetup::GetInstance()->GetCommandList()->Close();
	assert(SUCCEEDED(hr));



	//コマンドをキックする
	//GPUにコマンドリストの実行を行わせる
	ID3D12CommandList* commandLists[] = { DirectXSetup::GetInstance()->GetCommandList().Get()};
	DirectXSetup::GetInstance()->m_commandQueue_->ExecuteCommandLists(1, commandLists);
	//GPUとOSに画面の交換を行うよう通知する



	DirectXSetup::GetInstance()->swapChain.m_pSwapChain->Present(1, 0);

	////GPUにSignalを送る
	//GPUの実行完了が目的
	//1.GPUに実行が完了したタイミングでFEnceに指定した値を書き込んでもらう
	//  GPUに対してSignalを発行する
	//	Signal・・・GPUの指定の場所までたどり着いたら、指定の値を書き込んでもらうお願いのこと
	//2.CPUではFenceに指定した値が書き込まれているかを確認する
	//3.指定した値が書き込まれていない場合は、書き込まれるまで待つ
	//Fenceの値を更新
	fenceValue_++;
	//GPUがここまでたどりついた時に、Fenceの値を代入するようSignalを送る
	DirectXSetup::GetInstance()->m_commandQueue_->Signal(DirectXSetup::GetInstance()->m_fence_.Get(), fenceValue_);
	

	//Fenceの値が指定したSignal値にたどりついているか確認する
	//GetCompletedValueの初期値はFence作成時に渡した初期値
	if (DirectXSetup::GetInstance()->m_fence_->GetCompletedValue() < fenceValue_) {
		//指定したSignalにたどりついていないので、たどり着くまで待つようにイベントを設定する
		DirectXSetup::GetInstance()->m_fence_->SetEventOnCompletion(fenceValue_, fenceEvent_);
		//イベントを待つ
		WaitForSingleObject(fenceEvent_, INFINITE);
	}
	
	UpdateFPS();


	hr = DirectXSetup::GetInstance()->m_commandAllocator_->Reset();
	assert(SUCCEEDED(hr));

	hr = DirectXSetup::GetInstance()->m_commandList_->Reset(DirectXSetup::GetInstance()->m_commandAllocator_.Get(), nullptr);
	assert(SUCCEEDED(hr));
}

void DirectXSetup::Release() {

	//解放処理
	CloseHandle(fenceEvent_);
}




