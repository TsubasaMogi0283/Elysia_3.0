#include "LevelEditorSample.h"
#include <imgui.h>
#include <Input.h>
#include <AdjustmentItems.h>
#include "GameScene/GameScene.h"

#include "GameManager.h"
#include "ModelManager.h"
#include "AnimationManager.h"
#include <numbers>
#include <TextureManager.h>


LevelEditorSample::LevelEditorSample(){
	//レベルエディタのインスタンスを取得
	levelEditor_ = LevelDataManager::GetInstance();
	//オーディオのインスタンスを取得
	audio_ = Audio::GetInstance();
}

void LevelEditorSample::Initialize(){


	

	//読み込み
	levelHandle_= levelEditor_->Load("Test/Test.json");

	//平行光源の初期化
	directionalLight_.Initialize();
	
	//マテリアルの初期化
	material_.Initialize();
	material_.lightingKinds_ = Directional;

	//カメラ
	camera_.Initialize();
	camera_.translate_ = {.x = 0.0f,.y = 2.0f,.z = -30.0f };

	//ポストエフェクト
	back_ = std::make_unique<Vignette>();
	back_->Initialize();


	//オーディオの読み込み
	audioHandle_ = audio_->LoadWave("Resources/Audio/Sample/Win.wav");
	audioHandle2_= audio_->LoadWave("Resources/Audio/Sample/Hit.wav");
	audioHandleMP3_ = audio_->LoadMP3("Resources/Audio/Sample/WIP.mp3");
	//audio_->PlayMP3(audioHandleMP3_, true);

}

void LevelEditorSample::Update(GameManager* gameManager){

#ifdef _DEBUG
	ImGui::Begin("Camera"); 
	ImGui::SliderFloat3("Position", &camera_.translate_.x, -50.0f, 50.0f);
	ImGui::SliderFloat3("Rotate", &camera_.rotate_.x , -3.0f, 3.0f);
	ImGui::End();

	ImGui::Begin("LevelEditor");
	ImGui::Text("Rキーで指定した再読み込みするよ");
	ImGui::End();

	if (Input::GetInstance()->IsTriggerKey(DIK_R) == true) {
		levelEditor_->Reload(levelHandle_);

	}
	

#endif // _DEBUG

	gameManager;

	//更新
	levelEditor_->Update(levelHandle_);

	material_.Update();
	directionalLight_.Update();
	camera_.Update();

}

void LevelEditorSample::DrawSpriteBack()
{
}

void LevelEditorSample::DrawObject3D(){
	levelEditor_->Draw(levelHandle_,camera_, material_, directionalLight_);
}

void LevelEditorSample::PreDrawPostEffectFirst(){
	back_->PreDraw();
}

void LevelEditorSample::DrawPostEffect(){
	back_->Draw();
}

void LevelEditorSample::DrawSprite(){


}

LevelEditorSample::~LevelEditorSample(){
}
