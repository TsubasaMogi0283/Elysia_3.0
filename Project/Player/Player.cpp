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

#pragma region キーボード
	XINPUT_STATE joyState{};
	const float MOVE_SPEED = 0.1f;
	Vector3 move = {};
	isPressKey_ = false;
	//移動
	if (Input::GetInstance()->IsPushKey(DIK_RIGHT) == true) {
		move.x = 1.0f;
		isPressKey_ = true;
	}
	else if (Input::GetInstance()->IsPushKey(DIK_LEFT) == true) {
		move.x = -1.0f;
		isPressKey_ = true;
	}
	else if (Input::GetInstance()->IsPushKey(DIK_UP) == true) {
		move.z = 1.0f;
		isPressKey_ = true;
	}
	else if (Input::GetInstance()->IsPushKey(DIK_DOWN) == true) {
		move.z = -1.0f;
		isPressKey_ = true;
	}
	
	
#pragma endregion




	//コントローラー
	
	//コントローラーがある場合
	if (Input::GetInstance()->GetJoystickState(joyState)==true) {
		if (isPressKey_ == false) {
			move.x += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * 1.0f;
			move.z += (float)joyState.Gamepad.sThumbRY / SHRT_MAX * 1.0f;

			//何か勝手に動いちゃうので制限を掛ける
			const float MOVE_LIMITATION = 0.07f;
			if (move.x < MOVE_LIMITATION && move.x > -MOVE_LIMITATION) {
				move.x = 0.0f;
			}
			if (move.z < MOVE_LIMITATION && move.z > -MOVE_LIMITATION) {
				move.z = 0.0f;
			}

		}
		

	}





#ifdef _DEBUG
	//ImGuiにInputUintが内の不便・・
	int keyQuantity = haveKeyQuantity_;
	ImGui::Begin("Player");
	ImGui::InputInt("KeyQuantity", &keyQuantity);
	ImGui::InputFloat3("Transrate", &worldTransform_.translate_.x);
	ImGui::InputFloat3("Move", &move.x);
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


