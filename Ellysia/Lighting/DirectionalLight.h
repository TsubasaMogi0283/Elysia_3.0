#pragma once
#include "Vector4.h" 
#include "Vector3.h"
#include "DirectXSetup.h"

struct DirectionalLightData {
	//ライトの色
	Vector4 color;
	//ライトの向き
	Vector3 direction;
	//輝度
	float intensity;
};



struct DirectionalLight {
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
	Vector4 color_ = {1.0f,1.0f,1.0f,1.0f};
	//ライトの向き
	Vector3 direction_ = {0.0f,-1.0f,0.0f};
	//輝度
	float intensity_=5.0f;

	//定数バッファ
	ComPtr<ID3D12Resource> bufferResource_=nullptr;
	//書き込みデータ
	DirectionalLightData* directionalLight_ = nullptr;

};
