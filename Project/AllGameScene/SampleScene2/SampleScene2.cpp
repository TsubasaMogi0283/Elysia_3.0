#include "SampleScene2.h"
#include <TextureManager.h>
#include <Input.h>
#include "SampleScene/SampleScene.h"
#include "GameManager.h"

SampleScene2::SampleScene2(){

}

void SampleScene2::Initialize() {
	uint32_t textureHandle = TextureManager::LoadTexture("Resources/monsterBall.png");
	sprite_.reset(Sprite::Create(textureHandle, { .x = 500.0f,.y = 0.0f }));
	sprite_->SetScale({0.5f, 0.5f});

	uint32_t textureHandle2 = TextureManager::LoadTexture("Resources/uvChecker.png");
	spriteUV_.reset(Sprite::Create(textureHandle2, { .x = 0.0f,.y = 400.0f }));


	uint32_t howToTexture = TextureManager::GetInstance()->LoadTexture("Resources/Sample/HowTo/NextScene.png");
	howToSprite_.reset(Sprite::Create(howToTexture, { .x = 0.0f,.y = 0.0f }));
}

void SampleScene2::Update(GameManager* gameManager){

	

	//次のシーンへ
	if (Input::GetInstance()->IsTriggerKey(DIK_SPACE) == true) {
		gameManager->ChangeScene(new SampleScene());

	}

}

void SampleScene2::PreDrawPostEffectFirst()
{
}



void SampleScene2::DrawSpriteBack()
{
}

void SampleScene2::DrawObject3D(){
	
}

void SampleScene2::DrawPostEffect()
{
}

void SampleScene2::DrawSprite(){
	sprite_->Draw();
	spriteUV_->Draw();
	howToSprite_->Draw();
}

SampleScene2::~SampleScene2()
{
}
