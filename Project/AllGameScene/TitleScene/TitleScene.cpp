#include "TitleScene.h"
#include <imgui.h>
#include <Input.h>
#include "GameScene/GameScene.h"

#include "GameManager.h"
#include "ModelManager.h"
#include "AnimationManager.h"
#include <numbers>
#include <TextureManager.h>


void TitleScene::Initialize(){

	uint32_t logoTextureHandle = TextureManager::GetInstance()->LoadTexture("Resources/Title/StartText.png");
	uint32_t titleTextureHandle = TextureManager::GetInstance()->LoadTexture("Resources/Title/Title.png");

	//初期化
	const Vector2 INITIAL_POSITION = {.x=0.0f,.y=0.0f};
	text_.reset(Sprite::Create(logoTextureHandle, INITIAL_POSITION));
	backGround_.reset(Sprite::Create(titleTextureHandle, INITIAL_POSITION));

	isStart_ = false;
	isFlash_ = true;
	isFastFlash_ = false;

	//カメラ
	camera_.Initialize();
	camera_.translate_ = { 0.0f,0.0f,-9.8f };
}

void TitleScene::Update(GameManager* gameManager){


	//まだボタンを押していない時
	//通常点滅
	if (isFlash_ == true) {
		flashTime_ += 1;

		if (flashTime_ > FLASH_TIME_LIMIT_ * 0 &&
			flashTime_ <= FLASH_TIME_LIMIT_ ) {
			text_->SetInvisible(false);
		}
		if (flashTime_ > FLASH_TIME_LIMIT_ &&
			flashTime_ <= FLASH_TIME_LIMIT_*2) {
			text_->SetInvisible(true);

		}
		if (flashTime_ > FLASH_TIME_LIMIT_*2) {
			flashTime_ = 0;
		}

	}
	

	//コントローラーのBを押すと高速点滅
	if (Input::GetInstance()->IsConnetGamePad() == true) {

		//Bボタンを押したとき
		if (Input::GetInstance()->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_B) {
			bTriggerTime_ += 1;

		}
		if ((Input::GetInstance()->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_B) == 0) {
			bTriggerTime_ = 0;
		}

		if (bTriggerTime_ == 1) {

			isFastFlash_ = true;
		}

	}

	//スペースを押したら高速点滅
	if (Input::GetInstance()->IsPushKey(DIK_SPACE) == true) {
		//脱出
		isFastFlash_ = true;
	}



	//高速点滅
	if (isFastFlash_ == true) {
		fastFlashTime_ += 1;
		if (fastFlashTime_ % FAST_FLASH_TIME_INTERVAL_ == 0) {
			//もう一度学び直したが
			//単純に+1にしたいなら前置インクリメント「++(名前)」がいいらしい

			//加算される前の値を入れたいなら後置インクリメント「(名前)++」にしよう
			++textDisplayCount_;
		}
		//表示
		if (textDisplayCount_ % 2 == 0) {
			text_->SetInvisible(true);
		}
		else {
			text_->SetInvisible(false);
		}


		//指定した時間を超えたらシーンチェンジ
		if (fastFlashTime_> FAST_FLASH_TIME_LIMIT_) {
			isStart_ = true;
		}
	}


	camera_.Update();

	//脱出
	if (isStart_ == true) {
		gameManager->ChangeScene(new GameScene());
	}
}

void TitleScene::DrawObject3D()
{
}

void TitleScene::PreDrawPostEffectFirst()
{
}

void TitleScene::DrawPostEffect()
{
}

void TitleScene::DrawSprite(){
	//背景
	backGround_->Draw();

	//テキスト
	text_->Draw();
	


}
