#include "Ground.h"

void Ground::Initialize(uint32_t modelhandle){
	model_.reset(Model::Create(modelhandle));
	worldTransform_.Initialize();
	const float SPHERE_SCALE = 40.0f;
	worldTransform_.scale_ = { SPHERE_SCALE,SPHERE_SCALE,SPHERE_SCALE };
	worldTransform_.translate_.x = 0.0f;
	worldTransform_.translate_.y = 0.0f;


	material_.Initialize();
	material_.lightingKinds_ = Spot;

}

void Ground::Update(){
	//更新
	worldTransform_.Update();
	material_.Update();

}

void Ground::Draw(Camera& camera, SpotLight& spotLight){
	model_->Draw(worldTransform_, camera, material_, spotLight);
}
