#include "LevelEditorSample.h"
#include <imgui.h>
#include <Input.h>
#include <AdjustmentItems.h>
#include "SampleScene/SampleScene.h"

#include "GameManager.h"
#include "ModelManager.h"
#include "AnimationManager.h"
#include <numbers>
#include <TextureManager.h>


void LevelEditorSample::Initialize(){


	levelEditor_ = new LevelDataManager();
	//levelEditor_->Load("Resources/LevelData/LevelData1", "TL1TestFor13.json");

	levelEditor_->Load("Test/Test.json");

	directionalLight_.Initialize();
	

	material_.Initialize();
	material_.lightingKinds_ = Directional;

	//カメラ
	camera_.Initialize();
	camera_.translate_ = { 0.0f,0.0f,-30.0f };

	back_ = std::make_unique<BackText>();
	back_->Initialize();


	//TL2
	uint32_t ellysiaTextureHandle = TextureManager::GetInstance()->LoadTexture("Resources/Sample/TL2/Ellysia.dds");
	Vector3 position = { .x = 0.0f,.y = 0.0f,.z = 0.0f };
	ellysia_.reset(Sprite::Create(ellysiaTextureHandle, position));

}

void LevelEditorSample::Update(GameManager* gameManager){


#ifdef _DEBUG
	ImGui::Begin("Camera"); 
	ImGui::SliderFloat3("Position", &camera_.translate_.x, -50.0f, 50.0f);
	ImGui::End();
#endif // _DEBUG



	gameManager;

	levelEditor_->Update();

	material_.Update();
	directionalLight_.Update();
	camera_.Update();

}

void LevelEditorSample::DrawSpriteBack()
{
}

void LevelEditorSample::DrawObject3D(){
	//levelEditor_->Draw(camera_, material_, directionalLight_);
}

void LevelEditorSample::PreDrawPostEffectFirst(){
	back_->PreDraw();
}

void LevelEditorSample::DrawPostEffect(){
	back_->Draw();
}

void LevelEditorSample::DrawSprite(){
	ellysia_->Draw();

}

LevelEditorSample::~LevelEditorSample(){
	delete levelEditor_;
}
