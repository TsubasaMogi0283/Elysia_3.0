#pragma once
#include "DirectXSetup.h"

class GrayScale{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	GrayScale() = default;


	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 描画前処理
	/// </summary>
	void PreDraw();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GrayScale() = default;


private:

	uint32_t rtvHandle_ = 0;
	//RTV
	ComPtr<ID3D12Resource> rtvResource_ = nullptr;

	uint32_t srvHandle_ = 0;

	D3D12_RESOURCE_BARRIER barrier = {};


};

