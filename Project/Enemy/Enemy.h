#pragma once
#include "Vector3.h"
#include "WorldTransform.h"
#include "Model.h"

struct Camera;

class Enemy{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Enemy()=default;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="modelHandle"></param>
	/// <param name="position"></param>
	void Initialize(uint32_t modelHandle, Vector3 position);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();


	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	void Draw(Camera& camera);


	/// <summary>
	/// デストラクタ
	/// </summary>
	~Enemy()=default;


public:
	void SetSpotLightPosition(Vector3 position) {
		model_->SetSpotLightPosition(position);
	}
	void SetSpotLightIntensity(float intencity) {
		model_->SetSpotLightIntensity(intencity);
	}
	void SetSpotLightDirection(Vector3 direction) {
		model_->SetSpotLightDirection(direction);

	}
	void SetSpotLightDistance(float distance) {
		model_->SetSpotLightDistance(distance);
	}
	void SetSpotLightDecay(float decay) {
		model_->SetSpotLightDecay(decay);
	}
	void SetCosFalloffStart(float fallOff) {
		model_->SetCosFalloffStart(fallOff);
	}
	void SetSpotLightCosAngle(float cosAngle) {
		model_->SetSpotLightCosAngle(cosAngle);

	}

	



private:
	WorldTransform worldTransform_ = {};
	std::unique_ptr<Model> model_ = nullptr;

};

