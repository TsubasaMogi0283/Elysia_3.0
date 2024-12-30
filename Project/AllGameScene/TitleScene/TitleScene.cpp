#include "TitleScene.h"
#include <imgui.h>
#include <numbers>

#include "Input.h"
#include "GameScene/GameScene.h"
#include "GameManager.h"
#include "ModelManager.h"
#include "AnimationManager.h"

#include "TextureManager.h"
#include "LevelDataManager.h"

TitleScene::TitleScene(){
	//テクスチャ管理クラスの取得
	textureManager_ = TextureManager::GetInstance();
	//入力クラスの取得
	input_ = Input::GetInstance();
	//レベルエディタ管理クラスの取得
	levelDataManager_ = LevelDataManager::GetInstance();
}

void TitleScene::Initialize(){

	//ロゴ
	uint32_t logoTextureHandle = textureManager_->LoadTexture("Resources/Title/StartText.png");
	//タイトルテクスチャ
	uint32_t titleTextureHandle = textureManager_->LoadTexture("Resources/Title/Title.png");

	//初期化
	const Vector2 INITIAL_POSITION = {.x=0.0f,.y=0.0f};
	//生成
	//テキスト
	text_.reset(Sprite::Create(logoTextureHandle, INITIAL_POSITION));
	//毎系
	backGround_.reset(Sprite::Create(titleTextureHandle, INITIAL_POSITION));


	levelHandle_ = levelDataManager_->Load("TitleStage/TitleStage.json");

	isStart_ = false;
	isFlash_ = true;
	isFastFlash_ = false;

	//マテリアルの初期化
	material_.Initialize();
	material_.lightingKinds_ = Spot;

	//スポットライトの初期化
	spotLight.Initialize();

	//カメラの初期化
	camera_.Initialize();
	//座標
	camera_.translate_ = {.x = 0.0f,.y = 0.0f,.z = -9.8f };
}

void TitleScene::Update(GameManager* gameManager){
	//増える時間の値
	const uint32_t INCREASE_VALUE = 1u;
	//Bトリガーの反応する時間
	const uint32_t REACT_TIME = 1u;
	//Bトリガーの反応しない時間
	const uint32_t NO_REACT_TIME = 0u;

	//再スタート時間
	const uint32_t RESTART_TIME = 0u;

	


	//まだボタンを押していない時
	//通常点滅
	if (isFlash_ == true) {
		
		//時間の加算
		flashTime_ += INCREASE_VALUE;

		if (flashTime_ > FLASH_TIME_LIMIT_ * 0 &&
			flashTime_ <= FLASH_TIME_LIMIT_ ) {
			text_->SetInvisible(false);
		}
		if (flashTime_ > FLASH_TIME_LIMIT_ &&
			flashTime_ <= FLASH_TIME_LIMIT_*2) {
			text_->SetInvisible(true);

		}
		if (flashTime_ > FLASH_TIME_LIMIT_*2) {
			flashTime_ = RESTART_TIME;
		}

	}
	

	//コントローラーのBを押すと高速点滅
	if (input_->IsConnetGamePad() == true) {

		//Bボタンを押したとき
		if (input_->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_B) {
			bTriggerTime_ += INCREASE_VALUE;

		}
		//押していない
		if ((input_->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_B) == 0) {
			bTriggerTime_ = NO_REACT_TIME;
		}

		//反応
		if (bTriggerTime_ == REACT_TIME) {

			isFastFlash_ = true;
		}

	}

	//スペースを押したら高速点滅
	if (input_->IsPushKey(DIK_SPACE) == true) {
		//高速点滅
		isFastFlash_ = true;
	}

	//カウントが増える時間
	const uint32_t INCREASE_COUNT_TIME = 0u;
	//点滅の間隔
	const uint32_t FLASH_INTERVAL = 2u;

	//高速点滅
	if (isFastFlash_ == true) {
		fastFlashTime_ += INCREASE_VALUE;
		if (fastFlashTime_ % FAST_FLASH_TIME_INTERVAL_ == INCREASE_COUNT_TIME) {
			//もう一度学び直したが
			//単純に+1にしたいなら前置インクリメント「++(名前)」がいいらしい

			//加算される前の値を入れたいなら後置インクリメント「(名前)++」にしよう
			++textDisplayCount_;
		}
		//表示
		const uint32_t DISPLAY_MOMENT = 0u;
		if (textDisplayCount_ % FLASH_INTERVAL == DISPLAY_MOMENT) {
			text_->SetInvisible(true);
		}
		//非表示
		else {
			text_->SetInvisible(false);
		}


		//指定した時間を超えたらシーンチェンジ
		if (fastFlashTime_> FAST_FLASH_TIME_LIMIT_) {
			isStart_ = true;
		}
	}
	//脱出
	if (isStart_ == true) {
		gameManager->ChangeScene(new GameScene());
		return;
	}


	//ステージデータの更新
	Listener listener = {
			.position = {},
			.move = {},
	};
	levelDataManager_->SetListener(levelHandle_, listener);

	levelDataManager_->Update(levelHandle_);

	//マテリアルの更新
	material_.Update();
	//スポットライトの更新
	spotLight.Update();

	//カメラの更新
	camera_.Update();

	
}

void TitleScene::DrawObject3D(){

	levelDataManager_->Draw(levelHandle_,camera_, material_, spotLight);


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
