#pragma once
#include "DirectXSetup.h"
#include <array>
#include <string>

class RtvManager{
private:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	RtvManager() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~RtvManager() = default;

public:
	
	/// <summary>
	/// インスタンスの取得
	/// </summary>
	/// <returns></returns>
	static RtvManager* GetInstance();

	/// <summary>
	/// コピーコンストラクタ禁止
	/// </summary>
	/// <param name="rtvHeapManager"></param>
	RtvManager(const RtvManager& rtvHeapManager) = delete;

	/// <summary>
	/// 代入演算子を無効にする
	/// </summary>
	/// <param name="rtvHeapManager"></param>
	/// <returns></returns>
	RtvManager& operator=(const RtvManager& rtvHeapManager) = delete;

public:
	/// <summary>
	/// RenderTextureを作る
	/// </summary>
	/// <param name="format"></param>
	/// <param name="clearColor"></param>
	/// <returns></returns>
	static ComPtr<ID3D12Resource> CreateRenderTextureResource(DXGI_FORMAT format, const Vector4 clearColor);


	/// <summary>
	/// RenderTextureを作る(Depth版)
	/// </summary>
	/// <param name="format"></param>
	/// <param name="clearColor"></param>
	/// <returns></returns>
	static ComPtr<ID3D12Resource> CreateRenderTextureResourceForDepth(DXGI_FORMAT format, const Vector4 clearColor);


	/// <summary>
	/// ディスクリプタヒープの取得
	/// </summary>
	/// <returns></returns>
	ComPtr<ID3D12DescriptorHeap> GetRtvDescriptorHeap() {
		return  m_rtvDescriptorHeap_;
	}


	// <summary>
	// ハンドルの取得
	// </summary>
	// <param name="number"></param>
	// <returns></returns>
	D3D12_CPU_DESCRIPTOR_HANDLE& GetRtvHandle(uint32_t number) {
		return rtvHandles_[number];
	}




public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// nameによって返すindexが変わる
	/// </summary>
	/// <param name="name"></param>
	/// <returns></returns>
	uint32_t Allocate(std::string name);

	/// <summary>
	/// RTV作成
	/// </summary>
	/// <param name="resource"></param>
	void GenarateRenderTargetView(ComPtr<ID3D12Resource> resource,uint32_t handle);



private:

	//ディスクリプタ
	static const uint32_t RTV_DESCRIPTOR_SIZE_ = 30;
	//インデックス
	uint32_t index_ = 0;
	ComPtr<ID3D12DescriptorHeap> m_rtvDescriptorHeap_ = nullptr;
	static D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles_[RTV_DESCRIPTOR_SIZE_];


	struct RTVInformation {
		//PostEffect名
		std::string name_;
		//index
		uint32_t index_ = 0;

	};

	std::array<RTVInformation, RTV_DESCRIPTOR_SIZE_ > rtvInformation_{};


};




