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


	//カメラ
	camera_.Initialize();
	camera_.translate_ = { 0.0f,0.0f,-9.8f };
}

void LevelEditorSample::Update(GameManager* gameManager){


	gameManager;


	camera_.Update();

}

void LevelEditorSample::DrawSpriteBack()
{
}

void LevelEditorSample::DrawObject3D()
{
}

void LevelEditorSample::PreDrawPostEffectFirst()
{
}

void LevelEditorSample::DrawPostEffect()
{
}

void LevelEditorSample::DrawSprite(){
	title_->Draw();

	
	text_->Draw();
	


}

LevelEditorSample::~LevelEditorSample()
{
}

LevelEditorSample::~LevelEditorSample()
{
}
