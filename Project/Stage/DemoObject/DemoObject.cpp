#include "DemoObject.h"


void DemoObject::Initialize(const uint32_t& modelHandle,const Vector3& position){
	model_.reset(Model::Create(modelHandle));

	//ワールドトランスフォーム
	worldTransform_.Initialize();
	worldTransform_.translate_ = position;

	const float INTERVAL = 1.0f;
	Vector3 min = { .x = INTERVAL,.y = INTERVAL,.z = INTERVAL };
	Vector3 max = { .x = INTERVAL,.y = INTERVAL,.z = INTERVAL };

	aabb_.min.x = position.x - min.x;
	aabb_.min.y = position.y - min.y;
	aabb_.min.z = position.z - min.z;

	aabb_.max.x = position.x + max.x;
	aabb_.max.y = position.y + max.y;
	aabb_.max.z = position.z + max.z;

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

AABB DemoObject::GetAABB(){
	return aabb_;
}

