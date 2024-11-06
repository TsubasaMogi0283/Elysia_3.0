#include "Skydome.h"

void Skydome::Initialize(uint32_t modelHandle){

	model_.reset(Model::Create(modelHandle));

	worldTransform_.Initialize();
	const float SCALE_SIZE = 200.0f;
	worldTransform_.scale = { .x = SCALE_SIZE ,.y = SCALE_SIZE ,.z = SCALE_SIZE };


	material_.Initialize();
	material_.lightingKinds_ = LightingType::Directional;
	directionalLight_.Initialize();

}

void Skydome::Update(){


	//更新
	worldTransform_.Update();
	material_.Update();
	directionalLight_.Update();

}

void Skydome::Draw(Camera& camera){
	model_->Draw(worldTransform_, camera, material_, directionalLight_);
}
