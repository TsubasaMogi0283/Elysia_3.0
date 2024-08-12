#include "DemoObject.h"


void DemoObject::Initialize(uint32_t& modelHandle){
	model_.reset(Model::Create(modelHandle));

	//ワールドトランスフォーム
	worldTransform_.Initialize();
}

void DemoObject::Update(){
	

	worldTransform_.Update();


	aabb_.min.x = GetWorldPosition().x - radius_;
	aabb_.min.y = GetWorldPosition().y - radius_;
	aabb_.min.z = GetWorldPosition().z - radius_;

	aabb_.max.x = GetWorldPosition().x + radius_;
	aabb_.max.y = GetWorldPosition().y + radius_;
	aabb_.max.z = GetWorldPosition().z + radius_;

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

