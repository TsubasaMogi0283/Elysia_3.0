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

/// <summary>
/// スワップチェインで使う変数をまとめた
/// </summary>
struct SwapChain {
	ComPtr<IDXGISwapChain4> m_pSwapChain;
	ComPtr<ID3D12Resource> m_pResource[2];
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
};



/// <summary>
/// リソースリークチェック
/// ReportLiveObjects
/// DirectX12より低レベルのDXGIに問い合わせをする
/// </summary>
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


/// <summary>
/// DirectXの機能をまとめたクラス
/// フレームワークなど
/// </summary>
class DirectXSetup final {
private:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	DirectXSetup() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~DirectXSetup() = default;

public:
	/// <summary>
	/// インスタンスの取得
	/// </summary>
	/// <returns></returns>
	static DirectXSetup* GetInstance();

	/// <summary>
	/// コピーコンストラクタ禁止
	/// </summary>
	/// <param name="directXSetup"></param>
	DirectXSetup(const DirectXSetup& directXSetup) = delete;

	/// <summary>
	/// 代入演算子を無効にする
	/// </summary>
	/// <param name="directXSetup"></param>
	/// <returns></returns>
	DirectXSetup& operator=(const DirectXSetup& directXSetup) = delete;

	
	/// <summary>
	/// DescriptorHeapの作成関数
	/// </summary>
	/// <param name="heapType"></param>
	/// <param name="numDescriptors"></param>
	/// <param name="shaderVisible"></param>
	/// <returns></returns>
	static ComPtr<ID3D12DescriptorHeap> GenarateDescriptorHeap(
		D3D12_DESCRIPTOR_HEAP_TYPE heapType,
		UINT numDescriptors, bool shaderVisible);



private:
	//DepthStencilTexture...奥行の根幹をなすものであり、非常に大量の読み書きを高速に行う必要がある
	//						Textureの中でも特に例外的な扱いが必要となっている

	/// <summary>
	/// DepthのResourceを生成する
	/// </summary>
	/// <param name="width"></param>
	/// <param name="height"></param>
	/// <returns></returns>
	static ComPtr<ID3D12Resource> GenerateDepthStencilTextureResource(const uint32_t& width, const uint32_t& height);


	//まとめたのが下の「Initialize」
	//他の所では使わないからprivateにしても良さそう
	//アロー演算子を使ったとき邪魔になるから


#pragma region 初期化について

	/// <summary>
	/// DXGIFactoryの生成
	/// </summary>
	static void GenerateDXGIFactory();

	/// <summary>
	/// アダプターの生成
	/// </summary>
	static void SelectAdapter();

	/// <summary>
	/// デバイスの生成
	/// </summary>
	static void GenerateD3D12Device();

	/// <summary>
	/// エラーが起こったら止める
	/// </summary>
	static void StopErrorWarning();

	/// <summary>
	/// コマンドの生成
	/// </summary>
	static void GenerateCommand();

	/// <summary>
	/// スワップチェーンの生成
	/// </summary>
	static void GenerateSwapChain();

	/// <summary>
	/// ディスクリプタヒープの生成
	/// </summary>
	static void GenarateDescriptorHeap();

	/// <summary>
	/// スワップチェーンを引っ張ってくる
	/// </summary>
	static void PullResourcesFromSwapChain();


	/// <summary>
	/// フェンスの生成
	/// </summary>
	static void GenarateFence();


public:
	/// <summary>
	/// ビューポートの生成
	/// </summary>
	/// <param name="width">横幅</param>
	/// <param name="height">縦幅</param>
	static void GenarateViewport(uint32_t width, uint32_t height);

	
	/// <summary>
	/// シザーの生成
	/// </summary>
	/// <param name="right">横幅</param>
	/// <param name="bottom">立幅</param>
	static void GenarateScissor(uint32_t right, uint32_t bottom);


	/// <summary>
	/// リソースバリアの設定
	/// </summary>
	/// <param name="resource"></param>
	/// <param name="beforeState"></param>
	/// <param name="afterState"></param>
	static void SetResourceBarrier(ComPtr<ID3D12Resource> resource, D3D12_RESOURCE_STATES beforeState, D3D12_RESOURCE_STATES afterState);
	
	/// <summary>
	/// リソースバリアの設定(スワップチェイン用)
	/// </summary>
	/// <param name="beforeState"></param>
	/// <param name="afterState"></param>
	static void SetResourceBarrierForSwapChain(D3D12_RESOURCE_STATES beforeState, D3D12_RESOURCE_STATES afterState);


#pragma endregion

private:
	/// <summary>
	/// FPS固定初期化
	/// </summary>
	static void InitializeFPS();

	/// <summary>
	/// FPS固定更新
	/// </summary>
	static void UpdateFPS();


public:
	/// <summary>
	/// 第一初期化
	/// </summary>
	static void FirstInitialize();

	/// <summary>
	/// 第二初期化
	/// </summary>
	static void SecondInitialize();

	/// <summary>
	/// Resource作成の関数化
	/// </summary>
	/// <param name="sizeInBytes"></param>
	/// <returns></returns>
	ComPtr<ID3D12Resource> CreateBufferResource(size_t sizeInBytes);

	/// <summary>
	/// Depthリソースの取得
	/// </summary>
	/// <returns></returns>
	ComPtr<ID3D12Resource> GerDepthStencilResource() {
		return m_depthStencilResource_;
	}

	
#pragma region whileの中身
	/// <summary>
	/// 描画開始
	/// </summary>
	void StartDraw();

	/// <summary>
	/// 描画衆力
	/// </summary>
	void EndDraw();

#pragma endregion


	/// <summary>
	/// 解放
	/// </summary>
	void Release();


public:


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

	//スワップチェイン
	SwapChain swapChain = {};
	UINT backBufferIndex_ = {};
	D3D12_RESOURCE_BARRIER barrier_{};


	//スワップチェーン1枚目
	uint32_t rtvHandle[2] = {};
	std::string swapChainName[2] = {};

	
	//Fence
	ComPtr<ID3D12Fence> m_fence_ = nullptr;
	uint64_t fenceValue_ = 0;
	HANDLE fenceEvent_ = nullptr;

	//デバッグコントローラー
	ComPtr<ID3D12Debug1> debugController_ = nullptr;




	//FPS
	//記録時間(FPS固定用)
	std::chrono::steady_clock::time_point frameEndTime_;

};
