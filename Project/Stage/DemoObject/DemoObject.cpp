#include "DemoObject.h"

void DemoObject::Initialize(){
	uint32_t modelHandle = ModelManager::GetInstance()->LoadModelFile("Resources/Sample/Cube", "cube.obj");
	model_.reset(Model::Create(modelHandle));


	worldTransform_.Initialize();
	worldTransform_.translate_.y = 1.0f;
	material_.Initialize();
	material_.lightingKinds_ = Spot;
}

void DemoObject::Update(){
	material_.Update();
	worldTransform_.Update();
}

void DemoObject::Draw(Camera& camera, SpotLight& spotLight){
	model_->Draw(worldTransform_, camera, material_, spotLight);
}
