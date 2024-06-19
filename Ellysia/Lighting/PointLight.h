#pragma once
#include <Vector4.h>
#include <Vector3.h>
#include "DirectXSetup.h"

struct PointLightData {
	//ライトの色
	Vector4 color;
	//ライトの位置
	Vector3 position;
	//輝度
	float intensity;

	//ライトに届く最大距離
	float radius;
	//減衰率
	float decay;
	float padding[2];

};


//点光源
struct PointLight {
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
	//色
	Vector4 color_= { 1.0f,1.0f,1.0f,1.0f };
	//位置
	Vector3 position_ = {0.0f,0.0f,0.0f};
	//輝度
	float intensity_=4.0f;
	//ライトに届く最大距離
	float radius_=5.0f;
	//減衰率
	float decay_=5.0f;

	//定数バッファ
	ComPtr<ID3D12Resource> bufferResource_=nullptr;
	//書き込みデータ
	PointLightData* pointLightdata_ = nullptr;


};