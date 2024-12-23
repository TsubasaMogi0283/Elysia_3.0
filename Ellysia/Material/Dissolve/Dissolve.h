#pragma once

/**
 * @file Dissolve.h
 * @brief ディゾルブ構造体
 * @author 茂木翼
 */

#include "DirectXSetup.h"
#include "Vector3.h"

/// <summary>
/// データ
/// </summary>
struct DissolveData {
	//Edgeを使うかどうか
	bool isUseEdge;
	float padding[2];
	//Edgeの色
	Vector3 edgeColor;
	//Edgeの厚さ
	float edgeThinkness;
	//閾値
	float threshold;
};

/// <summary>
/// ディゾルブ
/// </summary>
struct Dissolve {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();


	/// <summary>
	/// 更新
	/// </summary>
	void Update();


public:


	//リソース
	ComPtr<ID3D12Resource> resource_ = nullptr;
	//書き込み用のデータ
	DissolveData* dissolveData_ = {};
	//テクスチャ
	uint32_t texture_ = 0u;
	//Edgeを使うかどうか
	bool isUseEdge_;
	//Edgeの色
	Vector3 edgeColor_;
	//Edgeの厚さ
	float edgeThinkness_;
	//閾値
	float threshold_;



};