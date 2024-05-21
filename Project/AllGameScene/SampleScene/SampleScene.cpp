#include "SampleScene.h"
#include <imgui.h>
#include <Input.h>
#include <AdjustmentItems.h>

#include "ModelManager.h"
/// <summary>
	/// コンストラクタ
	/// </summary>
SampleScene::SampleScene() {

}



/// <summary>
/// 初期化
/// </summary>
void SampleScene::Initialize() {
	
	//GLTF2.0
	//「GLTF Separate(.gltf+bin+Texture)」、「オリジナルを保持」で
	//"C:\Lesson\CG\CGGrade3\Ellysia_3.0\Resources\LevelData\Sphere\Sphere.obj"


	modelHandle =ModelManager::GetInstance()->LoadModelFile("Resources/LevelData/Sphere", "Sphere.obj");


	model_.reset(Model::Create(modelHandle));
	
	worldTransform_.Initialize();
	camera_.Initialize();
	camera_.translate_.z = -30.0f;
	
	//levelDataManager_ = new LevelDataManager();
	//levelDataManager_->Load("Resources/LevelData/TL1Test.json");


}






/// <summary>
/// 更新
/// </summary>
void SampleScene::Update(GameManager* gameManager) {
	gameManager;

#ifdef _DEBUG
	ImGui::Begin("Camera");
	ImGui::SliderFloat3("Rotatet", &camera_.rotate_.x, -3.0f, 3.0f);
	ImGui::SliderFloat3("Translate", &camera_.translate_.x, -100.0f, 10.0f);
	ImGui::End();

#endif

	//levelDataManager_->Update();

	worldTransform_.Update();
	camera_.Update();


#ifdef _DEBUG
	ImGui::Begin("Model");
	ImGui::SliderFloat3("Translate", &worldTransform_.rotate_.x, -30.0f, 30.0f);
	ImGui::End();

#endif

	AdjustmentItems::GetInstance()->SaveFile(GroupName);
}

/// <summary>
/// 描画
/// </summary>
void SampleScene::Draw() {

	//levelDataManager_->Draw(camera_);

	//AnimationManagerを作った方が良いかも引数を増やすの嫌だ。
	Animation animation = ModelManager::GetInstance()->GetModelAnimation(modelHandle);
	model_->Draw(worldTransform_, camera_, animation);
}




/// <summary>
/// デストラクタ
/// </summary>
SampleScene::~SampleScene() {
	delete levelDataManager_;
}