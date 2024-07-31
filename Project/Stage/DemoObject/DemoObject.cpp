#include "DemoObject.h"


void DemoObject::Initialize(uint32_t& modelHandle){
	model_.reset(Model::Create(modelHandle));

	//ワールドトランスフォーム
	worldTransform_.Initialize();
	worldTransform_.translate_.y = 1.0f;
}

void DemoObject::Update(){
	worldTransform_.Update();
}

void DemoObject::Draw(Camera& camera, Material& material, SpotLight& spotLight){
	model_->Draw(worldTransform_, camera, material, spotLight);
}

Vector3 DemoObject::GetWorldPosition(){
	Vector3 position = {
		.x = worldTransform_.worldMatrix_.m[3][0],
		.y = worldTransform_.worldMatrix_.m[3][1],
		.z = worldTransform_.worldMatrix_.m[3][2],
	};
	return position;
}

