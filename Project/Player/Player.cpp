#include "Player.h"
#include <Input.h>
#include <VectorCalculation.h>

void Player::Initialize(){
	uint32_t modelHandle = ModelManager::GetInstance()->LoadModelFile("Resources/CG3/Sphere", "Sphere.obj");
	model_.reset(Model::Create(modelHandle));


	worldTransform_.Initialize();

}

void Player::Update(){


	const float MOVE_SPEED = 0.1f;

	Vector3 move = {};

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
	worldTransform_.translate_ = Add(worldTransform_.translate_, { move.x * MOVE_SPEED,move.y * MOVE_SPEED,move.z * MOVE_SPEED });

	
	worldTransform_.Update();
}

void Player::Draw(Camera& camera){

	model_->Draw(worldTransform_, camera);
}
