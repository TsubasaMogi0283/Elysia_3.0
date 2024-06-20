#pragma once
#include "Vector4.h"
#include <cstdint>
#include "Matrix4x4.h"
#include "DirectXSetup.h"
#include "LightingType.h"


struct MaterialData {
	Vector4 color;
	int32_t lightingKinds;
	float padding[3];
	Matrix4x4 uvTransform;
	float shininess;
	bool isEnviromentMap;
};

//モデル用のマテリアルの設定
struct Material {
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
	Vector4 color_ = {1.0f,1.0f,1.0f,1.0f};
	//Lightingの種類
	int32_t lightingKinds_=Directional;
	//UV行列
	Matrix4x4 uvTransform_ = {};
	//輝度
	float shininess_=100.0f;
	//環境マップ
	bool isEnviromentMap_ = false;

	//定数バッファ
	ComPtr<ID3D12Resource> bufferResource_ = nullptr;
	//書き込みのデータ
	MaterialData* materialData_ = nullptr;

};
