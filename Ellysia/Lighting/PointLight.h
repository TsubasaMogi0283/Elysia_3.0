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
	Vector4 color_;
	//位置
	Vector3 position_;
	//輝度
	float intensity_;
	//ライトに届く最大距離
	float radius_;
	//減衰率
	float decay_;
	

	//定数バッファ
	ComPtr<ID3D12Resource> bufferResource_;
	//書き込みデータ
	PointLightData* pointLightdata_ = nullptr;


};