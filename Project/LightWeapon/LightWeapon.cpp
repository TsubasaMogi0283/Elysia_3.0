#include "LightWeapon.h"

#include <VectorCalculation.h>
#include "../Collider/CollisionConfig.h"
#include "Material.h"
#include "SpotLight.h"
#include "Camera.h"

void LightWeapon::Initialize(uint32_t modelHandle){

	model_.reset(Model::Create(modelHandle));
	color_ = { 1.0f,1.0f,1.0f,1.0f };
	lightColor_ = { 1.0f,1.0f,1.0f,1.0f };
	worldTransform_.Initialize();
	const float SCALE = 1.0f;
	worldTransform_.scale_ = { SCALE ,SCALE ,SCALE };
	radius_ = 1.0f * SCALE;

	DISTANCE_OFFSET = 26.0f;
	isCollision_ = false;
	
	//自分
	SetCollisionAttribute(COLLISION_ATTRIBUTE_PLAYER);
	//相手
	SetCollisionMask(COLLISION_ATTRIBUTE_ENEMY);


}


void LightWeapon::Update(Vector3 cameraPosition){

#ifdef _DEBUG


	ImGui::Begin("LightWeapon");
	ImGui::SliderFloat("Distance", &DISTANCE_OFFSET, 0.0f, 50.0f);
	ImGui::SliderFloat4("Color", &color_.x, 0.0f, 1.0f);
	ImGui::SliderFloat4("LightColor", &lightColor_.x, 0.0f, 1.0f);
	ImGui::End();

#endif

	worldTransform_.translate_ = Add(cameraPosition, { 0.0f,0.0f,DISTANCE_OFFSET });
	
	worldTransform_.Update();

}

void LightWeapon::OnCollision() {
#ifdef _DEBUG
	ImGui::Begin("LightWeapon");
	ImGui::End();
#endif // _DEBUG


	
	isCollision_ = true;
}

Vector3 LightWeapon::GetWorldPosition() {
	Vector3 result = {};
	result.x = worldTransform_.worldMatrix_.m[3][0];
	result.y = worldTransform_.worldMatrix_.m[3][1];
	result.z = worldTransform_.worldMatrix_.m[3][2];

	return result;
}


void LightWeapon::Draw(Camera& camera, Material& material, SpotLight& spotLight){
	model_->Draw(worldTransform_, camera,material,spotLight);
}

