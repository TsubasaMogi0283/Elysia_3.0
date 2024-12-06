#include "WinScene.h"
#include <TextureManager.h>

#include "TitleScene/TitleScene.h"
#include "GameScene/GameScene.h"
#include "GameManager.h"


WinScene::WinScene(){

	//入力のインスタンスを取得
	input_ = Input::GetInstance();
}

void WinScene::Initialize() {
	//スプライトの初期位置
	const Vector2 INITIAL_SPRITE_POSITION = { .x = 0.0f,.y = 0.0f };

	//脱出成功のテクスチャ
	uint32_t succeededTextureHandle = TextureManager::LoadTexture("Resources/Sprite/Result/Win/EscapeSucceeded.png");
	//生成
	succeeded_.reset(Sprite::Create(succeededTextureHandle, INITIAL_SPRITE_POSITION));

	//テキスト(タイトルへのやつ)
	uint32_t textHandle = TextureManager::GetInstance()->LoadTexture("Resources/Sprite/Result/Win/WinText.png");
	text_.reset(Sprite::Create(textHandle, INITIAL_SPRITE_POSITION));

	//黒背景
	uint32_t blackTexturehandle = TextureManager::GetInstance()->LoadTexture("Resources/Sprite/Back/Black.png");
	//生成
	black_.reset(Sprite::Create(blackTexturehandle, INITIAL_SPRITE_POSITION));
	//透明度の設定
	transparency_ = 0.0f;
	black_->SetTransparency(transparency_);
	
	//ブラックアウトの時間を初期化
	blackOutTime_ = 0;



	//点滅
	isFlash_ = true;

	//再開
	restart_ = false;

}

void WinScene::Update(GameManager* gameManager){

	//通常の点滅
	flashTime_ += 1;
	//表示
	if (flashTime_ > FLASH_TIME_LIMIT_ * 0 &&
		flashTime_ <= FLASH_TIME_LIMIT_) {
		text_->SetInvisible(false);
	}
	//非表示
	if (flashTime_ > FLASH_TIME_LIMIT_ &&
		flashTime_ <= FLASH_TIME_LIMIT_ * 2) {
		text_->SetInvisible(true);

	}
	//循環
	if (flashTime_ > FLASH_TIME_LIMIT_ * 2) {
		flashTime_ = 0;
	}

	//コントローラーを繋いでいる時
	if (input_->IsConnetGamePad() == true) {

		//Bボタンを押したとき
		if (input_->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_B) {
			bTriggerTime_ += 1;

		}
		//押していない時
		if ((input_->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_B) == 0) {
			bTriggerTime_ = 0;
		}

		//トリガー
		if (bTriggerTime_ == 1) {
			restart_ = true;
		}
	}
	//次のシーンへ
	if (input_->IsTriggerKey(DIK_SPACE) == true) {
		restart_ = true;
	}

	//再スタート
	if (restart_ == true) {

		//時間を足していく
		fastFlashTime_ += 1;
		if (fastFlashTime_ % FAST_FLASH_TIME_INTERVAL_ == 0) {
			++textDisplayCount_;
		}
		//表示
		if (textDisplayCount_ % 2 == 0) {
			text_->SetInvisible(true);
		}
		//非表示
		else {
			text_->SetInvisible(false);
		}


		//指定した時間を超えたらタイトルへ遷移
		if (fastFlashTime_ > FAST_FLASH_TIME_LIMIT_) {
			text_->SetInvisible(true);
			//透明度の設定
			black_->SetTransparency(transparency_);

			//暗くなったらシーンチェンジ
			transparency_ += TRANSPARENCY_INTERVAL;
		}

		//指定した時間まで時間が足される
		if (transparency_ > COMPLETELY_NO_TRANSPARENT_) {
			blackOutTime_ += 1;
		}

		
	}

	//タイトルシーンへ
	if (blackOutTime_ > CHANGE_TO_TITLE_TIME_) {
		gameManager->ChangeScene(new TitleScene());
		return;
	}

	


}

void WinScene::PreDrawPostEffectFirst(){

}



void WinScene::DrawObject3D(){
	
}

void WinScene::DrawPostEffect(){

}

void WinScene::DrawSprite(){
	//勝ち
	succeeded_->Draw();

	//テキスト
	text_->Draw();


	//フェードイン
	black_->Draw();


}

WinScene::~WinScene(){

}
