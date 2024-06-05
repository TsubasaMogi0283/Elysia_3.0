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


	D3D12_RENDER_TARGET_VIEW_DESC GetRtvDesc() {
		return rtvDesc_;
	}

	/// <summary>
	/// ハンドルの取得
	/// </summary>
	/// <param name="number"></param>
	/// <returns></returns>
	D3D12_CPU_DESCRIPTOR_HANDLE& GetRtvHandle(uint32_t number) {
		return rtvHandles_[number];
	}


#pragma region PostEffect用リソース

	ComPtr<ID3D12Resource> GetRenderTextureResource() {
		return renderTextureResource;
	}
	ComPtr<ID3D12Resource> GetOutLineTextureResource() {
		return outLineTextureResource;
	}
	ComPtr<ID3D12Resource> GetDepthBasedOutlineResource() {
		return depthBasedOutlineResource_;
	}

#pragma endregion



public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();




private:

	//ディスクリプタ
	static const uint32_t RTV_DESCRIPTOR_SIZE_ = 10;
	ComPtr<ID3D12DescriptorHeap> m_rtvDescriptorHeap_ = nullptr;
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc_{};
	D3D12_CPU_DESCRIPTOR_HANDLE rtvStartHandle_;
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles_[RTV_DESCRIPTOR_SIZE_] = {};
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescriptorHeapDesc_{};


	//PostEffect
	//いつか分解する
	ComPtr<ID3D12Resource> renderTextureResource = nullptr;
	//LuminanceBasedOutline用
	ComPtr<ID3D12Resource> outLineTextureResource = nullptr;
	//DepthBasedOutline用
	ComPtr<ID3D12Resource> depthBasedOutlineResource_ = nullptr;


};

