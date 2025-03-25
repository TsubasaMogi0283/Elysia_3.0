#include "PlayerCamera.h"

#include <numbers>
#include <imgui.h>

#include "GlobalVariables.h"
#include "VectorCalculation.h"
#include "Matrix4x4Calculation.h"



PlayerEyeCamera::PlayerEyeCamera(){
	//インスタンスの取得
	//グローバル変数
	globalVariables_ = Elysia::GlobalVariables::GetInstance();
}

void PlayerEyeCamera::Initialize(){

	//カメラの調整項目
	globalVariables_->CreateGroup(GAME_SCENE_CAMERA_NAME_);
	globalVariables_->AddItem(GAME_SCENE_CAMERA_NAME_, HEIGHT_OFFSET_, cameraPositionOffset_);

	//カメラ座標のオフセットの初期化
	cameraPositionOffset_ = { .x = 0.0f,.y = 1.2f,.z = 0.0f };
}

void PlayerEyeCamera::Update(){

	//位置の計算
	worldTransform_.translate = VectorCalculation::Add(playerPosition_, cameraPositionOffset_);

	//カメラの回転の計算
	worldTransform_.rotate.x = phi_;
	worldTransform_.rotate.y = -(theta_)+std::numbers::pi_v<float_t> / 2.0f;
	worldTransform_.rotate.z = 0.0f;


	//ワールド行列を計算
	worldTransform_.worldMatrix = Matrix4x4Calculation::MakeAffineMatrix(worldTransform_.scale, worldTransform_.rotate, worldTransform_.translate);
	//ビュー行列の計算
	camera_.viewMatrix = Matrix4x4Calculation::Inverse(worldTransform_.worldMatrix);


#ifdef _DEBUG
	ImGui::Begin("プレイヤーのカメラ");
	ImGui::InputFloat3("座標", &worldTransform_.translate.x);
	ImGui::InputFloat3("回転", &worldTransform_.rotate.x);
	ImGui::End();

#endif // _DEBUG


}
