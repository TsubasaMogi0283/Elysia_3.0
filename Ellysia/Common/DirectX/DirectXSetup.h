#pragma once

#include "ConvertLog.h"
#include "WindowsSetup.h"
#include "Vector4.h"


#include <cassert>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <dxgidebug.h>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dxcompiler.lib")

#include <wrl.h>
using Microsoft::WRL::ComPtr;


#include <chrono>

struct SwapChain {
	ComPtr<IDXGISwapChain4> m_pSwapChain;
	ComPtr<ID3D12Resource> m_pResource[2];
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
};

////ReportLiveObjects
	//DirectX12より低レベルのDXGIに問い合わせをする
	//リソースリークチェック
struct D3DResourceLeakChecker {
	~D3DResourceLeakChecker() {
		ComPtr<IDXGIDebug1>debug;
		if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&debug))))
		{
			debug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
			debug->ReportLiveObjects(DXGI_DEBUG_APP, DXGI_DEBUG_RLO_ALL);
			debug->ReportLiveObjects(DXGI_DEBUG_D3D12, DXGI_DEBUG_RLO_ALL);

		}



	}
};


//メンバ変数関数いつか整理したい・・・
//ごちゃごちゃしてる
class DirectXSetup final {
private:

	//コンストラクタ
	DirectXSetup() = default;

	//デストラクタ
	~DirectXSetup() = default;

public:
	//インスタンス
	static DirectXSetup* GetInstance();

	//コピーコンストラクタ禁止
	DirectXSetup(const DirectXSetup& directXSetup) = delete;

	//代入演算子を無効にする
	DirectXSetup& operator=(const DirectXSetup& directXSetup) = delete;

	//まとめたのが下の「Initialize」
	//他の所では使わないからprivateにしても良さそう
	//アロー演算子を使ったとき邪魔になるから

	//DescriptorHeapの作成関数
	static ComPtr<ID3D12DescriptorHeap> GenarateDescriptorHeap(
		D3D12_DESCRIPTOR_HEAP_TYPE heapType,
		UINT numDescriptors, bool shaderVisible);



private:
	//DepthStencilTexture...奥行の根幹をなすものであり、非常に大量の読み書きを高速に行う必要がある
	//						Textureの中でも特に例外的な扱いが必要となっている
	static ComPtr<ID3D12Resource> CreateDepthStencilTextureResource(const int32_t width, const int32_t height);


#pragma region 初期化について
	//初期化へ

	static void GenerateDXGIFactory();

	static void SelectAdapter();

	static void GenerateD3D12Device();

	static void StopErrorWarning();

	static void GenerateCommand();

	static void GenerateSwapChain();

	static void MakeDescriptorHeap();

	static void PullResourcesFromSwapChain();








	static void SetRTV();


public:
	static void GenarateViewport();

	static void GenerateScissor();


#pragma endregion

private:
	//FPS固定初期化
	static void InitializeFPS();

	//FPS固定更新
	static void UpdateFPS();



public:

	static void FirstInitialize();

	static void SecondInitialize();

	//Resource作成の関数化
	ComPtr<ID3D12Resource> CreateBufferResource(size_t sizeInBytes);

	ComPtr<ID3D12Resource> GerDepthStencilResource() {
		return m_depthStencilResource_;
	}

	
#pragma region whileの中身
	
	void StartDraw();


	void EndDraw();

#pragma endregion


#pragma region 解放処理
	//解放
	void Release();
#pragma endregion



private:



public:



#pragma region 他のクラスでも使いたいのでGetter

	/// <summary>
	/// デバイスの取得
	/// </summary>
	/// <returns></returns>
	ComPtr<ID3D12Device> GetDevice() {
		return m_device_;
	}

	/// <summary>
	/// コマンドリストの取得
	/// </summary>
	/// <returns></returns>
	ComPtr<ID3D12GraphicsCommandList> GetCommandList() {
		return DirectXSetup::GetInstance()->m_commandList_;
	}

	/// <summary>
	/// DSVディスクリプタヒープの取得
	/// </summary>
	/// <returns></returns>
	ComPtr<ID3D12DescriptorHeap> GetDsvDescriptorHeap() {
		return  m_dsvDescriptorHeap_;
	}


	/// <summary>
	/// スワップチェーンの取得
	/// </summary>
	/// <returns></returns>
	SwapChain GetSwapChain() {
		return DirectXSetup::GetInstance()->swapChain;
	}



	/// <summary>
	/// DSVハンドルの取得
	/// </summary>
	/// <returns></returns>
	D3D12_CPU_DESCRIPTOR_HANDLE& GetDsvHandle() {
		return dsvHandle_;
	}


#pragma endregion



private:

	//デバイス
	ComPtr<IDXGIFactory7> m_dxgiFactory_ = nullptr;
	ComPtr<IDXGIAdapter4> m_useAdapter_ = nullptr;
	ComPtr<ID3D12Device> m_device_ = nullptr;

	//コマンド
	ComPtr<ID3D12GraphicsCommandList> m_commandList_ = nullptr;
	ComPtr<ID3D12CommandQueue> m_commandQueue_ = nullptr;
	ComPtr<ID3D12CommandAllocator> m_commandAllocator_ = nullptr;

	//DSV
	ComPtr<ID3D12DescriptorHeap> m_dsvDescriptorHeap_ = nullptr;
	ComPtr<ID3D12Resource> m_depthStencilResource_ = nullptr;
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle_ = {};

	//SwapChain
	SwapChain swapChain = {};
	
	D3D12_RESOURCE_BARRIER barrier_{};

	//Fence
	ComPtr<ID3D12Fence> m_fence_ = nullptr;
	uint64_t fenceValue_ = 0;
	HANDLE fenceEvent_ = nullptr;


	ComPtr<ID3D12Debug1> debugController_ = nullptr;




	//FPS
	//記録時間(FPS固定用)
	std::chrono::steady_clock::time_point reference_;




};
