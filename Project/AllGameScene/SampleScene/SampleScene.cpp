#include "SampleScene.h"
#include <imgui.h>
#include <Input.h>
#include <AdjustmentItems.h>
#include "SampleScene2/SampleScene2.h"

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
	//modelHandle =ModelManager::GetInstance()->LoadModelFile("Resources/CG4/AnimatedCube", "AnimatedCube.gltf",false);
	modelHandle = ModelManager::GetInstance()->LoadModelFile("Resources/CG3/Sphere", "Sphere.obj");



	model_.reset(Model::Create(modelHandle));

	Matrix4x4 localMatrix = ModelManager::GetInstance()->GetModelData(modelHandle).rootNode.localMatrix;

	worldTransform_.Initialize();
	camera_.Initialize();


	audio_ = Audio::GetInstance();
	//audioHandle_ = audio_->LoadMP3(L"Resources/Audio/Sample/WIP.mp3");

	audioHandle_ = audio_->LoadWave("Resources/Audio/Sample/Game.wav");

	//audio_->PlayWave(audioHandle_,false);
	audio_->SetPan(audioHandle_, pan_);
	audio_->ChangePitch(audioHandle_, pitch_);


	audio_->SendChannels(audioHandle_, 1);

	uint32_t textureHandle = TextureManager::GetInstance()->LoadTexture("Resources/White.png");
	sprite_.reset(Sprite::Create(textureHandle, { 0.0f,0.0f }));
	sprite_->SetScale({0.5f, 0.5f});
	
	//back_ = new BackText();
	//back_->Initialize();


	//outLine_ = new LuminanceBasedOutline();
	//outLine_->Initialize();
	depthBasedOutline_ = new DepthBasedOutline();
	depthBasedOutline_->Initialize();

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
	worldTransform_.Update();
	camera_.Update();

	sprite_->SetPosition(position);
#ifdef _DEBUG
	ImGui::Begin("Audio");
	ImGui::SliderFloat("Pan", &pan_, -1.0f, 1.0f);
	ImGui::SliderFloat("LowPassFilter", &cutOff_, 0.0f, 1.0f);

	ImGui::InputInt("Pitch", &pitch_);
	ImGui::End();

	ImGui::Begin("Camera");
	ImGui::SliderFloat3("Position", &camera_.translate_.x, -40.0f, 20.0f);
	ImGui::End();


	ImGui::Begin("Sprite");
	ImGui::SliderFloat3("Z",&position.x,0.0f,1280.0f);
	ImGui::End();


	ImGui::Begin("Model");
	ImGui::SliderFloat3("Translate", &worldTransform_.rotate_.x, -30.0f, 30.0f);
	ImGui::End();

#endif
	if (Input::GetInstance()->IsTriggerKey(DIK_SPACE) == true) {
		AdjustmentItems::GetInstance()->SaveFile(GroupName);
		gameManager->ChangeScene(new SampleScene2());
	}
	
}

void SampleScene::DrawSpriteBack(){

	sprite_->Draw();
}

void SampleScene::PreDrawPostEffectFirst(){
	
	//outLine_->PreDraw();
	//back_->PreDraw();
	depthBasedOutline_->PreDraw();
}


void SampleScene::DrawObject3D() {
	model_->Draw(worldTransform_, camera_);
}



void SampleScene::PreDrawPostEffectSecond(){
	
	//outLine_->PreDrawSecond();
	//back_->PostDraw();
	depthBasedOutline_->PreDrawSecond();
}

void SampleScene::DrawPostEffect(){
	//outLine_->Draw();
	//back_->Draw();
	depthBasedOutline_->Draw();
}

void SampleScene::DrawSprite(){
	
}




/// <summary>
/// デストラクタ
/// </summary>
SampleScene::~SampleScene() {
	delete back_;
	delete outLine_;
	delete depthBasedOutline_;
}