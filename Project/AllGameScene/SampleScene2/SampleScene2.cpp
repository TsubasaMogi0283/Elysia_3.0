#include "SampleScene2.h"
#include <TextureManager.h>
#include <Input.h>
#include "TitleScene/TitleScene.h"
#include "GameScene/GameScene.h"
#include "GameManager.h"


void SampleScene2::Initialize() {
	//メイン
	uint32_t succeededTextureHandle = TextureManager::LoadTexture("Resources/Result/Win/EscapeSucceeded.png");
	winTexture_.reset(Sprite::Create(succeededTextureHandle, { .x = 0.0f,.y = 0.0f }));
	winTexture_->SetScale({ .x = 1.0f,.y = 1.0f });

	//Text
	uint32_t textHandle = TextureManager::GetInstance()->LoadTexture("Resources/Result/Win/WinText.png");
	text_.reset(Sprite::Create(textHandle, { .x = 0.0f,.y = 0.0f }));

	uint32_t blackTexturehandle = TextureManager::GetInstance()->LoadTexture("Resources/Back/Black.png");
	black_.reset(Sprite::Create(blackTexturehandle, { .x = 0.0f,.y = 0.0f }));
	transparency_ = 0.0f;
	black_->SetTransparency(transparency_);
	blackOutTime_ = 0;



	//点滅
	isFlash_ = true;

	//
	restart_ = false;

}

void SampleScene2::Update(GameManager* gameManager){


	//通常の点滅
	flashTime_ += 1;
	if (flashTime_ > FLASH_TIME_LIMIT_ * 0 &&
		flashTime_ <= FLASH_TIME_LIMIT_) {
		text_->SetInvisible(false);
	}
	if (flashTime_ > FLASH_TIME_LIMIT_ &&
		flashTime_ <= FLASH_TIME_LIMIT_ * 2) {
		text_->SetInvisible(true);

	}
	if (flashTime_ > FLASH_TIME_LIMIT_ * 2) {
		flashTime_ = 0;
	}


	if (Input::GetInstance()->IsConnetGamePad() == true) {

		//Bボタンを押したとき
		if (input_->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_B) {
			bTriggerTime_ += 1;

		}
		if ((input_->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_B) == 0) {
			bTriggerTime_ = 0;
		}

		if (bTriggerTime_ == 1) {
			
			restart_ = true;
		}
	}
	//次のシーンへ
	if (Input::GetInstance()->IsTriggerKey(DIK_SPACE) == true) {
		restart_ = true;
	}


	if (restart_ == true) {

		fastFlashTime_ += 1;
		if (fastFlashTime_ % FAST_FLASH_TIME_INTERVAL_ == 0) {
			++textDisplayCount_;
		}
		//表示
		if (textDisplayCount_ % 2 == 0) {
			text_->SetInvisible(true);
		}
		else {
			text_->SetInvisible(false);
		}


		//指定した時間を超えたらタイトルへ遷移
		if (fastFlashTime_ > FAST_FLASH_TIME_LIMIT_) {
			text_->SetInvisible(true);
			//透明度の設定
			black_->SetTransparency(transparency_);

			//暗くなったらシーンチェンジ
			const float TRANSPARENCY_INTERVAL = 0.01f;
			transparency_ += TRANSPARENCY_INTERVAL;
		}


		if (transparency_ > 1.0f) {
			blackOutTime_ += 1;
		}

		
	}

	if (blackOutTime_ > 60*2) {
		gameManager->ChangeScene(new TitleScene());
		return;
	}

	


}

void SampleScene2::PreDrawPostEffectFirst(){

}

void SampleScene2::DrawSpriteBack(){

}

void SampleScene2::DrawObject3D(){
	
}

void SampleScene2::DrawPostEffect(){

}

void SampleScene2::DrawSprite(){
	winTexture_->Draw();
	text_->Draw();


	//フェードイン
	black_->Draw();


}

SampleScene2::~SampleScene2(){

}
