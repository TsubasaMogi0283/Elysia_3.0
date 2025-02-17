#pragma once
/**
 * @file SpotLight.h
 * @brief スポットライトのクラス
 * @author 茂木翼
 */

#include "Vector4.h"
#include "Vector3.h"
#include "DirectXSetup.h"


/// <summary>
/// スポットライト
/// </summary>
struct SpotLight {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();


private:
	/// <summary>
	/// スポットライトのデータ
	/// </summary>
	struct SpotLightData {
		//ライトの色
		Vector4 color;
		//ライトの位置
		Vector3 position;
		//輝度
		float intensity;

		//スポットライトの方向
		Vector3 direction;
		//ライトの届く最大距離
		float distance;
		//減衰率
		float decay;
		//Fallowoffを制御する
		float cosFallowoffStart;
		//スポットライトの余弦
		float cosAngle;

		//当たっていない所をこれで明るくする
		float aroundOffset;

		float padding[2];

	};


public:
	//ライトの色
	Vector4 color;
	//ライトの位置
	Vector3 position;
	//輝度
	float intensity;

	//方向
	Vector3 direction;
	//届く最大距離
	float distance;
	//減衰率
	float decay;
	//Fallowoffを制御する
	float cosFallowoffStart;
	//スポットライトの余弦
	float cosAngle;
	//当たっていない所をこれで明るくする
	float aroundOffset;

	//定数バッファ
	ComPtr<ID3D12Resource> resource=nullptr;
	//書き込みデータ
	SpotLightData* data_ = nullptr;


};
