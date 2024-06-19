#pragma once
#include <Vector4.h>
#include <Vector3.h>
#include "DirectXSetup.h"

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
	float padding[2];


};


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


public:
	//ライトの色
	Vector4 color_;
	//ライトの位置
	Vector3 position_;
	//輝度
	float intensity_;

	//スポットライトの方向
	Vector3 direction_;
	//ライトの届く最大距離
	float distance_;
	//減衰率
	float decay_;
	//Fallowoffを制御する
	float cosFallowoffStart_;
	//スポットライトの余弦
	float cosAngle_;

	//定数バッファ
	ComPtr<ID3D12Resource> bufferResource_=nullptr;
	//書き込みデータ
	SpotLightData* spotLightData_ = nullptr;


};
