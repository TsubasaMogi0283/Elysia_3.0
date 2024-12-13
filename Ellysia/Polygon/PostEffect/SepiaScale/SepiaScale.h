#pragma once
/**
 * @file SepiaScale.h
 * @brief セピアのクラス
 * @author 茂木翼
 */

#include "DirectXSetup.h"

/// <summary>
/// セピア
/// </summary>
class SepiaScale{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	SepiaScale() = default;


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
	~SepiaScale() = default;


private:
	//RTVハンドル
	uint32_t rtvHandle_ = 0;
	//RTV
	ComPtr<ID3D12Resource> rtvResource_ = nullptr;
	//SRVハンドル
	uint32_t srvHandle_ = 0;
	//バリア
	D3D12_RESOURCE_BARRIER barrier = {};


};

