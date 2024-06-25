#include "LightWeapon.h"

#include <VectorCalculation.h>
#include "../Collider/CollisionConfig.h"

#include "SpotLight.h"
#include "Camera.h"

void LightWeapon::Initialize(uint32_t modelHandle){
	//モデル
	model_.reset(Model::Create(modelHandle));
	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	const float SCALE = 1.0f;
	worldTransform_.scale_ = { SCALE ,SCALE ,SCALE };
	//マテリアルの初期化
	material_.Initialize();
	material_.lightingKinds_ = None;
	
	
	
	
	radius_ = 1.0f * SCALE;

	distanceOffset_ = 25.0f;
	//衝突
	isCollision_ = false;
	
	//自分
	SetCollisionAttribute(COLLISION_ATTRIBUTE_PLAYER);
	//相手
	SetCollisionMask(COLLISION_ATTRIBUTE_ENEMY);


}


void LightWeapon::OnCollision() {
#ifdef _DEBUG
	ImGui::Begin("LightWeapon");
	ImGui::End();
#endif

	//衝突した
	isCollision_ = true;
}

Vector3 LightWeapon::GetWorldPosition() {
	Vector3 result = {};
	result.x = worldTransform_.worldMatrix_.m[3][0];
	result.y = worldTransform_.worldMatrix_.m[3][1];
	result.z = worldTransform_.worldMatrix_.m[3][2];

	return result;
}


void LightWeapon::Update(Vector3 playerPosition, Vector3 direction){

#ifdef _DEBUG
	ImGui::Begin("LightWeapon");
	ImGui::SliderFloat("Distance", &distanceOffset_, 0.0f, 50.0f);
	ImGui::End();

#endif


	//座標
	Vector3 newTranslate = {};
	newTranslate.x = distanceOffset_ * direction.x;
	newTranslate.y = distanceOffset_ * direction.y+3.0f;
	newTranslate.z = distanceOffset_ * direction.z;

	//加算
	worldTransform_.translate_ = Add(playerPosition, newTranslate);
	
	//更新
	worldTransform_.Update();
	material_.Update();
}



void LightWeapon::Draw(Camera& camera, SpotLight& spotLight){
	model_->Draw(worldTransform_, camera,material_,spotLight);
}

