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

	levelEditor_->Load("Test/LevelEditor.json");

	directionalLight_.Initialize();
	

	material_.Initialize();
	material_.lightingKinds_ = Directional;

	//カメラ
	camera_.Initialize();
	camera_.translate_ = { 0.0f,0.0f,-9.8f };
}

void LevelEditorSample::Update(GameManager* gameManager){


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
	levelEditor_->Draw(camera_, material_, directionalLight_);
}

void LevelEditorSample::PreDrawPostEffectFirst(){
	back_->PreDraw();
}

void LevelEditorSample::DrawPostEffect()
{
}

void LevelEditorSample::DrawSprite(){


}

LevelEditorSample::~LevelEditorSample()
{
}
