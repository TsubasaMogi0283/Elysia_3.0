#pragma once
#include "DirectXSetup.h"
#include <array>
#include <string>

class RtvManager{
private:
	//コンストラクタ
	RtvManager() = default;

	//デストラクタ
	~RtvManager() = default;

public:
	//インスタンス
	static RtvManager* GetInstance();

	//コピーコンストラクタ禁止
	RtvManager(const RtvManager& rtvHeapManager) = delete;

	//代入演算子を無効にする
	RtvManager& operator=(const RtvManager& rtvHeapManager) = delete;

public:
	/// <summary>
	/// RenderTextureを作る
	/// </summary>
	/// <param name="width"></param>
	/// <param name="height"></param>
	/// <param name="format"></param>
	/// <param name="clearColor"></param>
	/// <returns></returns>
	static ComPtr<ID3D12Resource> CreateRenderTextureResource(uint32_t width, uint32_t height, DXGI_FORMAT format, const Vector4 clearColor);

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


	uint32_t Allocate(std::string name);

	/// <summary>
	/// RTV作成
	/// </summary>
	/// <param name="resource"></param>
	void GenarateRenderTargetView(ComPtr<ID3D12Resource> resource,uint32_t handle);



private:

	//ディスクリプタ
	static const uint32_t RTV_DESCRIPTOR_SIZE_ = 10;
	//インデックス
	uint32_t index_ = 0;
	ComPtr<ID3D12DescriptorHeap> m_rtvDescriptorHeap_ = nullptr;
	D3D12_CPU_DESCRIPTOR_HANDLE rtvStartHandle_;
	static D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles_[RTV_DESCRIPTOR_SIZE_];


	struct RTVInformation {
		//PostEffect名
		std::string name_;
		//index
		uint32_t index_ = 0;

	};

	std::array<RTVInformation, RTV_DESCRIPTOR_SIZE_ > rtvInformation_{};


};




