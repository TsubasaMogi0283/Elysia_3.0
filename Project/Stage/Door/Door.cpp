#include "Door.h"

#include <numbers>
#include <imgui.h>

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
	maxRotateY_ = initialRotateY_ + (std::numbers::pi_v<float_t>*3.0f) / 4.0f;
	//初期回転
	rotateY_ = initialRotateY_;
}


void Door::Update() {
	//距離を計算
	Vector3 doorDefference = VectorCalculation::Subtract(player_->GetWorldPosition(), levelDataManager_->GetInitialTranslate(levelDataHandle_, DOOR_STRING_));
	distance_ = SingleCalculation::Length(doorDefference);
	if (distance_ < MAX_DISTANCE_ && isOpen_ == false) {
		//スペース化Bボタンで開ける
		if (input_->IsTriggerKey(DIK_SPACE) == true || input_->IsTriggerButton(XINPUT_GAMEPAD_B) == true) {
			isOpen_ = true;
		}
	}

	//開いた状態
	if (isOpen_ == true) {
		//線形補間で開ける
		rotateT_ += ROTATE_VALUE_;
		rotateT_ = std::clamp(rotateT_, MIN_T_VALUE_, MAX_T_VALUE_);
		rotateY_ = SingleCalculation::Lerp(initialRotateY_, maxRotateY_, rotateT_);
	}

	//設定
	levelDataManager_->SetRotate(levelDataHandle_, DOOR_STRING_, { .x = 0.0f,.y = rotateY_,.z = 0.0f });

#ifdef _DEBUG
	ImGui::Begin("ドア");
	ImGui::InputFloat("距離", &distance_);
	ImGui::End();
#endif // _DEBUG


}


void Door::DrawSprite() {

}