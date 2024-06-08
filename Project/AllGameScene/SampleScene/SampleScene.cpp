#include "SampleScene.h"
#include <imgui.h>
#include <Input.h>
#include <AdjustmentItems.h>

#include "ModelManager.h"
#include "AnimationManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
SampleScene::SampleScene() {

}



/// <summary>
/// 初期化
/// </summary>
void SampleScene::Initialize() {
	




	//球
	uint32_t groundModelHandle = ModelManager::GetInstance()->LoadModelFile("Resources/CG3/Sphere", "Sphere.obj");
	ground_.reset(Model::Create(groundModelHandle));
	groundWorldTransform_.Initialize();
	const float SPHERE_SCALE = 1.0f;
	groundWorldTransform_.scale_ = { SPHERE_SCALE,SPHERE_SCALE,SPHERE_SCALE };
	groundWorldTransform_.translate_.x = 0.0f;
	groundWorldTransform_.translate_.y = 0.0f;



	camera_.Initialize();
}






/// <summary>
/// 更新
/// </summary>
void SampleScene::Update(GameManager* gameManager) {
	gameManager;

	
	groundWorldTransform_.Update();
	camera_.Update();

#ifdef _DEBUG


	ImGui::Begin("Camera");
	ImGui::SliderFloat3("Translate", &camera_.translate_.x, -100.0f, 100.0f);
	ImGui::SliderFloat3("Rotate", &camera_.rotate_.x, -3.0f, 3.0f);
	ImGui::End();

#endif
	if (Input::GetInstance()->IsTriggerKey(DIK_SPACE) == true) {
		AdjustmentItems::GetInstance()->SaveFile(GroupName);
	}
	
}

/// <summary>
/// 描画
/// </summary>
void SampleScene::Draw() {
	ground_->Draw(groundWorldTransform_,camera_);
	
}




/// <summary>
/// デストラクタ
/// </summary>
SampleScene::~SampleScene() {
	
}