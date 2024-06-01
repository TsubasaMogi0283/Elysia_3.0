#pragma once
#include "DirectXSetup.h"

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


	static ComPtr<ID3D12Resource> CreateRenderTextureResource(uint32_t width, uint32_t height, DXGI_FORMAT format, const Vector4 clearColor);


	ComPtr<ID3D12DescriptorHeap> GetRtvDescriptorHeap() {
		return  m_rtvDescriptorHeap_;
	}


	D3D12_RENDER_TARGET_VIEW_DESC GetRtvDesc() {
		return rtvDesc_;
	}

	D3D12_CPU_DESCRIPTOR_HANDLE& GetRtvHandle(uint32_t number) {
		return rtvHandles_[number];
	}

	ComPtr<ID3D12Resource> GetRenderTextureResource() {
		return renderTextureResource;
		

	}


public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();




private:

	//ディスクリプタ
	const uint32_t RTV_DESCRIPTOR_SIZE_ = 3;
	ComPtr<ID3D12DescriptorHeap> m_rtvDescriptorHeap_ = nullptr;
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc_{};
	D3D12_CPU_DESCRIPTOR_HANDLE rtvStartHandle_;
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles_[3] = {};
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescriptorHeapDesc_{};



	ComPtr<ID3D12Resource> renderTextureResource = nullptr;

};

