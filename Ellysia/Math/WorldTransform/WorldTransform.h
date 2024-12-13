#pragma once
/**
 * @file WorldTransform.h
 * @brief ワールドトランスフォーム
 * @author 茂木翼
 */

#include "Matrix4x4.h"
#include "Vector3.h"

#include "DirectXSetup.h"

/// <summary>
/// シェーダーに送るデータ
/// </summary>
struct WorldTransformData {
	Matrix4x4 world;
	Matrix4x4 normal;
	Matrix4x4 worldInverseTranspose;
};

/// <summary>
/// ワールドトランスフォーム
/// </summary>
struct WorldTransform {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// ペアレントの設定
	/// </summary>
	/// <param name="parent"></param>
	inline void SetParent(const WorldTransform* newParent) {
		parent = newParent;
	}

	/// <summary>
	/// ワールド座標を取得
	/// </summary>
	/// <returns></returns>
	inline Vector3 GetWorldPosition()const {
		Vector3 position = {
			.x = worldMatrix.m[3][0],
			.y = worldMatrix.m[3][1],
			.z = worldMatrix.m[3][2],
		};

		return position;
	}


private:
	/// <summary>
	/// 転送
	/// </summary>
	void Transfer();



public:

	//スケール
	Vector3 scale = { 1.0f, 1.0f, 1.0f };
	//回転
	Vector3 rotate = { 0.0f, 0.0f, 0.0f };
	//座標
	Vector3 translate = { 0.0f, 0.0f, 0.0f };


	//定数バッファ
	ComPtr<ID3D12Resource> bufferResource;
	//送るデータ
	WorldTransformData* tranceformationData = nullptr;

	//ワールド行列
	Matrix4x4 worldMatrix = {};
	//逆転置行列
	Matrix4x4 worldInverseTransposeMatrix = {};

	//親となるワールド変換へのポインタ
	const WorldTransform* parent = nullptr;


};

