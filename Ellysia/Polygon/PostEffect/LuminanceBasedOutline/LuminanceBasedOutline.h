#pragma once

/**
 * @file LuminanceBasedOutline.h
 * @brief アウトライン(輝度)の管理クラス
 * @author 茂木翼
 */


#include "DirectXSetup.h"
#include "Vector4.h"
#include "Vector3.h"
#include "VertexData.h"

/// <summary>
/// アウトライン(輝度)
/// </summary>
class LuminanceBasedOutline{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	LuminanceBasedOutline()=default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 描画前の処理
	/// </summary>
	void PreDraw();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	
	/// <summary>
	/// デストラクタ
	/// </summary>
	~LuminanceBasedOutline()=default;

private:

	//SRV
	uint32_t srvHandle_ = 0;

	//RTV
	ComPtr<ID3D12Resource> rtvResource_ = nullptr;
	uint32_t rtvHandle_ = 0;
	D3D12_RESOURCE_BARRIER barrier = {};
};

