#include "LightWeapon.h"
#include "WorldTransform.h"
#include <VectorCalculation.h>

void LightWeapon::Initialize(uint32_t modelHandle){

	model_.reset(Model::Create(modelHandle));
	color_ = { 1.0f,1.0f,1.0f,1.0f };
	lightColor_ = { 1.0f,1.0f,1.0f,1.0f };
	model_->SetLighting(Directional);
	
	worldTransform_.Initialize();
	const float SCALE = 1.0f;
	worldTransform_.scale_ = { SCALE ,SCALE ,SCALE };
	radius_ = 1.0f * SCALE;

	DISTANCE_OFFSET = 26.0f;
	isCollision_ = false;
}


void LightWeapon::Update(Vector3 cameraPosition){
	model_->SetColor(color_);
	model_->SetDirectionalKightColor(lightColor_);
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

void LightWeapon::Draw(Camera& camera){
	model_->Draw(worldTransform_, camera);
}

void LightWeapon::OnCollision(){
	isCollision_ = true;
}

Vector3 LightWeapon::GetWorldPosition(){
	return Vector3();
}
