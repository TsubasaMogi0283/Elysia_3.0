#include "Player.h"
#include <Input.h>
#include <VectorCalculation.h>

#include "Material.h"
#include "SpotLight.h"


void Player::Initialize(){
	uint32_t modelHandle = ModelManager::GetInstance()->LoadModelFile("Resources/CG3/Sphere", "Sphere.obj");
	model_.reset(Model::Create(modelHandle));

	//持っている鍵の数
	haveKeyQuantity_ = 0;

	//半径
	radius_ = 1.0f;

	worldTransform_.Initialize();
	worldTransform_.translate_.z = -16.0f;
}

void Player::Update(){

#pragma region キーボード
	const float MOVE_SPEED = 0.1f;
	Vector3 move = {};
	//移動
	//コマンドパターンでも良さそう
	if (Input::GetInstance()->IsPushKey(DIK_RIGHT) == true) {
		move.x = 1.0f;
	}
	else if (Input::GetInstance()->IsPushKey(DIK_LEFT) == true) {
		move.x = -1.0f;
	}
	else if (Input::GetInstance()->IsPushKey(DIK_UP) == true) {
		move.z = 1.0f;
	}
	else if (Input::GetInstance()->IsPushKey(DIK_DOWN) == true) {
		move.z = -1.0f;
	}


#pragma endregion





#ifdef _DEBUG
	//ImGuiにInputUintが内の不便・・
	int keyQuantity = haveKeyQuantity_;
	ImGui::Begin("Player");
	ImGui::InputInt("KeyQuantity", &keyQuantity);
	ImGui::InputFloat3("Transrate", &worldTransform_.translate_.x);
	ImGui::End();

#endif

	


	worldTransform_.translate_ = Add(worldTransform_.translate_, { move.x * MOVE_SPEED,move.y * MOVE_SPEED,move.z * MOVE_SPEED });

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


