#include "Door.h"

#include <numbers>

#include <Input.h>
#include <LevelDataManager.h>
#include <SingleCalculation.h>
#include <VectorCalculation.h>
#include <Player/Player.h>

Door::Door(){
	//入力クラス
	input_ = Elysia::Input::GetInstance();
	//レベルエディタ管理クラス
	levelDataManager_ = Elysia::LevelDataManager::GetInstance();
}

void Door::Initialize() {
	//空の場合停止
	assert(player_);

	//初期回転を取得
	initialRotateY_ = levelDataManager_->GetInitialRotate(levelDataHandle_, "Door").y;
	//最大の回転の値を設定
	maxDoorRotateY_ = initialRotateY_ + (std::numbers::pi_v<float_t>*3.0f) / 4.0f;
	//初期回転
	doorRotateY_ = initialRotateY_;
}


void Door::Update() {
	//距離を計算
	Vector3 doorDefference = VectorCalculation::Subtract(player_->GetWorldPosition(), levelDataManager_->GetInitialTranslate(levelDataHandle_, "Door"));
	doorLength = SingleCalculation::Length(doorDefference);
	if (doorLength < 6.0f && isDoorOpen_ == false) {
		//スペース化Bボタンで開ける
		if (input_->IsTriggerKey(DIK_SPACE) == true || input_->IsTriggerButton(XINPUT_GAMEPAD_B) == true) {
			isDoorOpen_ = true;
		}
	}

	//開いた状態
	if (isDoorOpen_ == true) {
		//線形補間で開ける
		rotateT_ += ROTATE_VALUE_;
		rotateT_ = std::clamp(rotateT_, MIN_T_VALUE_, MAX_T_VALUE_);
		doorRotateY_ = SingleCalculation::Lerp(initialRotateY_, maxDoorRotateY_, rotateT_);
	}

	//設定
	levelDataManager_->SetRotate(levelDataHandle_, "Door", { .x = 0.0f,.y = doorRotateY_,.z = 0.0f });
}


void Door::DrawSprite() {

}