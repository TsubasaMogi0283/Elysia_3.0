#include "Player.h"
#include <Input.h>
#include <VectorCalculation.h>

#include "Material.h"
#include "SpotLight.h"
#include <numbers>


void Player::Initialize(){
	uint32_t modelHandle = ModelManager::GetInstance()->LoadModelFile("Resources/Sample/TD3Player","Player.obj");
	model_.reset(Model::Create(modelHandle));

	//持っている鍵の数
	haveKeyQuantity_ = 0;

	//半径
	radius_ = 1.0f;

	worldTransform_.Initialize();
	worldTransform_.translate_.z = -16.0f;
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


