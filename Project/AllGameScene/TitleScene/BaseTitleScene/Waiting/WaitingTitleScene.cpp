#include "WaitingTitleScene.h"

#include "Input.h"
#include "TextureManager.h"
#include "LevelDataManager.h"

#include "TitleScene/TitleScene.h"
#include "TitleScene/BaseTitleScene/Noise/NoiseTitleScene.h"

WaitingTitleScene::WaitingTitleScene()
{
	//インスタンスの取得
	//テクスチャ管理クラス
	textureManager_ = Elysia::TextureManager::GetInstance();
	//入力クラス
	input_ = Elysia::Input::GetInstance();
	//レベルエディタ管理クラス
	levelDataManager_ = Elysia::LevelDataManager::GetInstance();
}

void WaitingTitleScene::Initialize(){

}

void WaitingTitleScene::Update(TitleScene* titleScene){
	//まだボタンを押していない時
	//通常点滅
	if (isFlash_ == true) {
		//再スタート時間
		const uint32_t RESTART_TIME = 0u;

		//時間の加算
		flashTime_ += INCREASE_VALUE;

		//表示
		if (flashTime_ > FLASH_TIME_LIMIT_ * 0u &&
			flashTime_ <= FLASH_TIME_LIMIT_ * 1u) {
			text_->SetInvisible(false);
		}
		//非表示
		if (flashTime_ > FLASH_TIME_LIMIT_ * 1u &&
			flashTime_ <= FLASH_TIME_LIMIT_ * 2u) {
			text_->SetInvisible(true);

		}
		if (flashTime_ > FLASH_TIME_LIMIT_ * 2u) {
			flashTime_ = RESTART_TIME;
		}


		//スペースをまたはBボタンを押したら高速点滅
		if (input_->IsPushKey(DIK_SPACE) == true || input_->IsTriggerButton(XINPUT_GAMEPAD_B) == true) {
			isFastFlash_ = true;
		}


	}



	

	//高速点滅
	if (isFastFlash_ == true) {
		//カウントが増える時間
		const uint32_t INCREASE_COUNT_TIME = 0u;

		//時間の加算
		fastFlashTime_ += INCREASE_VALUE;
		if (fastFlashTime_ % FAST_FLASH_TIME_INTERVAL_ == INCREASE_COUNT_TIME) {
			++textDisplayCount_;
		}

		//表示
		const uint32_t DISPLAY_MOMENT = 0u;

		//点滅の間隔
		const uint32_t FLASH_INTERVAL = 2u;

		if (textDisplayCount_ % FLASH_INTERVAL == DISPLAY_MOMENT) {
			text_->SetInvisible(true);
		}
		//非表示
		else {
			text_->SetInvisible(false);
		}


		//指定した時間を超えたらシーンチェンジ
		if (fastFlashTime_ > FAST_FLASH_TIME_LIMIT_) {
			//ノイズへ
			titleScene->ChangeDetailedScene(std::make_unique<NoiseTitleScene>());
			return;
		}
	}

}

void WaitingTitleScene::DrawObject3D()
{
}

void WaitingTitleScene::PreDrawPostEffect()
{
}

void WaitingTitleScene::DrawPostEffect()
{
}

void WaitingTitleScene::DrawSprite(){
	//背景
	logo->Draw(logoTextureHandle_);
	//テキスト
	text_->Draw();
}

WaitingTitleScene::~WaitingTitleScene()
{
}
