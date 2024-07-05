#include "Player.h"
#include <Input.h>
#include <VectorCalculation.h>

#include "Material.h"
#include "SpotLight.h"
#include <numbers>


#include "SampleScene/SampleScene.h"

void Player::Initialize(){
	uint32_t modelHandle = ModelManager::GetInstance()->LoadModelFile("Resources/Sample/TD3Player","Player.obj");
	model_.reset(Model::Create(modelHandle));


	//持っている鍵の数
	haveKeyQuantity_ = 0;

	//半径
	radius_ = 1.0f;

	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
}

void Player::Update(){

	




#ifdef _DEBUG
	//ImGuiにInputUintが内の不便・・
	int keyQuantity = haveKeyQuantity_;
	ImGui::Begin("Player");
	ImGui::InputInt("KeyQuantity", &keyQuantity);
	ImGui::InputFloat3("Transrate", &worldTransform_.translate_.x);
	ImGui::InputFloat3("MoveDirection", &moveDirection_.x);
	ImGui::End();

#endif


	

	const float MOVE_SPEED = 0.1f;
	//加算
	worldTransform_.translate_ = Add(worldTransform_.translate_, { moveDirection_.x * MOVE_SPEED,moveDirection_.y * MOVE_SPEED,moveDirection_.z * MOVE_SPEED });

	//ステージの外には行けないようにする
	//左
	if (worldTransform_.translate_.x < stageRect_.leftBack.x + radius_) {
		worldTransform_.translate_.x = stageRect_.leftBack.x + radius_;
	}
	//右
	if (worldTransform_.translate_.x > stageRect_.rightBack.x - radius_) {
		worldTransform_.translate_.x = stageRect_.rightBack.x - radius_;
	}
	//奥
	if (worldTransform_.translate_.z > stageRect_.leftBack.z - radius_) {
		worldTransform_.translate_.z = stageRect_.leftBack.z - radius_;
	}
	//手前
	if (worldTransform_.translate_.z < stageRect_.leftFront.z + radius_) {
		worldTransform_.translate_.z = stageRect_.leftFront.z + radius_;
	}

	//ワールドトランスフォームの更新
	worldTransform_.Update();



}

void Player::Draw(Camera& camera, Material& material, SpotLight& spotLight){
	
	model_->Draw(worldTransform_, camera,material,spotLight);
}

Vector3 Player::GetWorldPosition()const {
	Vector3 result = {};
	result.x = worldTransform_.worldMatrix_.m[3][0];
	result.y = worldTransform_.worldMatrix_.m[3][1];
	result.z = worldTransform_.worldMatrix_.m[3][2];

	return result;
}


