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
	modelHandle =ModelManager::GetInstance()->LoadModelFile("Resources/CG4/AnimatedCube", "AnimatedCube.gltf");

	model_.reset(Model::Create(modelHandle));
	
	Matrix4x4 localMatrix = ModelManager::GetInstance()->GetModelData(modelHandle).rootNode.localMatrix;

	worldTransform_.Initialize(true,localMatrix);
	camera_.Initialize();


	audio_ = Audio::GetInstance();
	//audioHandle_ = audio_->LoadMP3(L"Resources/Audio/Sample/WIP.mp3");

	audioHandle_ = audio_->LoadWave("Resources/Audio/Sample/Game.wav");

	audio_->PlayWave(audioHandle_,false);
	audio_->SetPan(audioHandle_, pan_);
	audio_->ChangePitch(audioHandle_, pitch_);


	audio_->SendChannels(audioHandle_, 1);

	
	
}






/// <summary>
/// 更新
/// </summary>
void SampleScene::Update(GameManager* gameManager) {
	gameManager;

	audio_->SetLowPassFilter(audioHandle_, cutOff_);
	audio_->ChangePitch(audioHandle_, pitch_);
	audio_->SetPan(audioHandle_, pan_);

	if (Input::GetInstance()->IsTriggerKey(DIK_SPACE) == true) {
		audio_->ExitLoop(audioHandle_);
	}

	if (Input::GetInstance()->IsTriggerKey(DIK_1) == true) {
		audio_->PauseWave(audioHandle_);
	}
	if (Input::GetInstance()->IsTriggerKey(DIK_2) == true) {
		audio_->ResumeWave(audioHandle_);
	}

	

	Matrix4x4 localMatrix = model_->GetAnimationLocalMatrix();
	worldTransform_.Update(localMatrix);
	camera_.Update();


#ifdef _DEBUG
	ImGui::Begin("Audio");
	ImGui::SliderFloat("Pan", &pan_, -1.0f, 1.0f);
	ImGui::SliderFloat("LowPassFilter", &cutOff_, 0.0f, 1.0f);

	ImGui::InputInt("Pitch", &pitch_);
	ImGui::End();



	ImGui::Begin("Model");
	ImGui::SliderFloat3("Translate", &worldTransform_.rotate_.x, -30.0f, 30.0f);
	ImGui::End();

#endif

}

/// <summary>
/// 描画
/// </summary>
void SampleScene::Draw() {
	model_->Draw(worldTransform_, camera_,ModelManager::GetInstance()->GetModelAnimation(modelHandle));
}




/// <summary>
/// デストラクタ
/// </summary>
SampleScene::~SampleScene() {
	AdjustmentItems::GetInstance()->SaveFile(GroupName);
}