#include "TitleScene.h"
#include <imgui.h>
#include <numbers>
#include <array>

#include "Input.h"
#include "GameManager.h"
#include "ModelManager.h"
#include "AnimationManager.h"
#include "TextureManager.h"
#include "LevelDataManager.h"
#include "VectorCalculation.h"
#include "SingleCalculation.h"
#include "Calculation/QuaternionCalculation.h"

#include "SunsetBackTexture.h"
#include "NightBackTexture.h"


TitleScene::TitleScene(){
	//テクスチャ管理クラスの取得
	textureManager_ = Ellysia::TextureManager::GetInstance();
	//入力クラスの取得
	input_ = Ellysia::Input::GetInstance();
	//レベルエディタ管理クラスの取得
	levelDataManager_ = Ellysia::LevelDataManager::GetInstance();
}

void TitleScene::Initialize(){

	//テクスチャハンドルの取得
	uint32_t textTextureHandle = textureManager_->LoadTexture("Resources/Sprite/Title/StartText.png");
	//タイトルテクスチャ
	uint32_t logoTextureHandle = textureManager_->LoadTexture("Resources/Sprite/Title/TitleNormal.png");
	changedLogoTextureHandle_ = textureManager_->LoadTexture("Resources/Sprite/Title/TitleChanged.png");
	logoTextureHandle_ = logoTextureHandle;
	//黒フェード
	uint32_t blackTexureHandle= textureManager_->LoadTexture("Resources/Sprite/Back/Black.png");

	//初期座標
	const Vector2 INITIAL_POSITION = {.x=0.0f,.y=0.0f};
	
	//テキスト
	text_.reset(Ellysia::Sprite::Create(textTextureHandle, INITIAL_POSITION));
	//ロゴ
	logo.reset(Ellysia::Sprite::Create(logoTextureHandle_, INITIAL_POSITION));
	//黒フェード
	blackFade_.reset(Ellysia::Sprite::Create(blackTexureHandle, INITIAL_POSITION));
	//初期の透明度設定
	const float INITIAL_TRANSPARENCY = 0.0f;
	blackFade_->SetTransparency(INITIAL_TRANSPARENCY);

	//レベルデータの読み込み
	levelHandle_ = levelDataManager_->Load("TitleStage/TitleStage.json");


	isStart_ = false;
	isFlash_ = true;
	isFastFlash_ = false;

	//スポットライトの初期化
	spotLight.Initialize();

	directionalLight_.Initialize();
	directionalLight_.color = { .x = 1.0f,.y = 0.22f,.z = 0.0f,.w = 1.0f };
	directionalLight_.direction = { .x = 0.91f,.y = -1.0f,.z = 0.0f };

	//カメラの初期化
	camera_.Initialize();
	//座標
	camera_.translate = {.x = 0.0f,.y = 0.0f,.z = -30.8f };

	//レールカメラ
	titleRailCamera_ = std::make_unique<TitleRailCamera>();
	//初期化
	titleRailCamera_->Initialize();
	

	//背景
	//ポストエフェクト
	baseTitleBackTexture_ = std::make_unique<SunsetBackTexture>();
	baseTitleBackTexture_->Initialize();

	//ランダムエフェクトの生成
	randomEffect_ = std::make_unique<Ellysia::RandomEffect>();
	//初期化
	randomEffect_->Initialize();
	
}

void TitleScene::Update(Ellysia::GameManager* gameManager){

#pragma region 未スタート

	//まだボタンを押していない時
	//通常点滅
	if (isFlash_ == true) {
		//再スタート時間
		const uint32_t RESTART_TIME = 0u;

		//時間の加算
		flashTime_ += INCREASE_VALUE;

		//表示
		if (flashTime_ > FLASH_TIME_LIMIT_ * 0 &&
			flashTime_ <= FLASH_TIME_LIMIT_ ) {
			text_->SetInvisible(false);
		}
		//非表示
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

		//Bトリガーの反応しない時間
		const uint32_t NO_REACT_TIME = 0u;
		//Bトリガーの反応する時間
		const uint32_t REACT_TIME = 1u;


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


	//高速点滅
	if (isFastFlash_ == true&& isStart_==false) {
		//カウントが増える時間
		const uint32_t INCREASE_COUNT_TIME = 0u;

		//時間の加算
		fastFlashTime_ += INCREASE_VALUE;
		if (fastFlashTime_ % FAST_FLASH_TIME_INTERVAL_ == INCREASE_COUNT_TIME) {
			//もう一度学び直したが
			//単純に+1にしたいなら前置インクリメント「++(名前)」がいいらしい
			//加算される前の値を入れたいなら後置インクリメント「(名前)++」にしよう
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
		if (fastFlashTime_> FAST_FLASH_TIME_LIMIT_) {
			isStart_ = true;
		}
	}

#pragma endregion

#pragma region スタート演出

	//シーン遷移演出
	if (isStart_ == true) {

		//スタートのテキストを非表示にさせる
		logo->SetInvisible(false);
		text_->SetInvisible(true);

		//時間の加算
		const float DELTA_TIME = 1.0f/60.0f;
		randomEffectTime_ += DELTA_TIME;

		//開始時間
		std::array<float, DISPLAY_LENGTH_QUANTITY_> RANDOM_EFFECT_DISPLAY_START_TIME = { 0.0f,2.5f };
		//表示の長さ
		std::array<float, DISPLAY_LENGTH_QUANTITY_> RANDOM_EFFECT_DISPLAY_LENGTH = { 1.0f,3.0f };



		//通常はfalse
		//指定時間内に入ったらtrue
		isDisplayRandomEffect_ = false;
		for (uint32_t i = 0; i < DISPLAY_LENGTH_QUANTITY_; ++i) {
			if (randomEffectTime_ > RANDOM_EFFECT_DISPLAY_START_TIME[i] &&
				randomEffectTime_ <= RANDOM_EFFECT_DISPLAY_START_TIME[i] + RANDOM_EFFECT_DISPLAY_LENGTH[i]) {
				//ランダムエフェクトの表示
				isDisplayRandomEffect_ = true;
				logo->SetInvisible(true);
				break;
			}

			//2回目のエフェクト
			const uint32_t FIRST_EFFECT = 0u;
			//2回目のエフェクト
			const uint32_t SECOND_EFFECT = 1u;

			if (i == FIRST_EFFECT) {
				//夜へ遷移
				ChangeBackTexture(std::move(std::make_unique<NightBackTexture>()));
				//タイトルロゴの変化
				logoTextureHandle_ = changedLogoTextureHandle_;
			}
			else if (i == SECOND_EFFECT) {
				//ランダムの終了
				if (randomEffectTime_ > RANDOM_EFFECT_DISPLAY_START_TIME[SECOND_EFFECT] + RANDOM_EFFECT_DISPLAY_LENGTH[SECOND_EFFECT]) {
					isEndDisplayRandomEffect_ = true;
				}
			}
			

		}

		
		//ランダムエフェクト表示の演出が終わった場合
		if (isEndDisplayRandomEffect_ == true) {
			logo->SetInvisible(true);
			const float FADE_INCREASE_VALUE = 0.01f;
			blackFadeTransparency_ += FADE_INCREASE_VALUE;
			
		}

		//時間も兼ねている
		if (blackFadeTransparency_ > 2.0f) {
			gameManager->ChangeScene("Game");
			return;
		}
		
	}
#pragma endregion

	//黒フェードの透明度の変更
	blackFade_->SetTransparency(blackFadeTransparency_);

	//更新
	levelDataManager_->Update(levelHandle_);

	//スポットライトの更新
	spotLight.Update();
	//平行光源
	directionalLight_.Update();

	//レールカメラの更新
	titleRailCamera_->Update();
	//レールカメラから2つの行列を取得
	camera_.viewMatrix = titleRailCamera_->GetCamera().viewMatrix;
	camera_.projectionMatrix = titleRailCamera_->GetCamera().projectionMatrix;

	//カメラの更新
	camera_.Transfer();




#ifdef _DEBUG
	//ImGui用
	DisplayImGui();
#endif
}

void TitleScene::DrawObject3D(){
	//ステージオブジェクト
	levelDataManager_->Draw(levelHandle_,camera_, directionalLight_);

}

void TitleScene::PreDrawPostEffectFirst(){

	//ランダム
	if (isDisplayRandomEffect_ == true) {
		//ランダム
		randomEffect_->PreDraw();
	}
	else {
		//背景
		baseTitleBackTexture_->PreDraw();
	}

}

void TitleScene::DrawPostEffect(){
	//ランダム
	if (isDisplayRandomEffect_ == true) {
		//ランダム
		randomEffect_->Draw();
	}
	else {
		//背景
		baseTitleBackTexture_->Draw();
	}
}

void TitleScene::DrawSprite(){
	//背景
	logo->Draw(logoTextureHandle_);

	//テキスト
	text_->Draw();
	
	//黒フェード
	blackFade_->Draw();

}

void TitleScene::DisplayImGui(){
	ImGui::Begin("TitleFade&Effect");
	ImGui::InputFloat("Count", &randomEffectTime_);
	ImGui::Checkbox("IsDisplay", &isDisplayRandomEffect_);
	ImGui::End();

	ImGui::Begin("平行光源");
	ImGui::SliderFloat4("色", &directionalLight_.color.x, 0.0f, 1.0f);
	ImGui::SliderFloat3("方向", &directionalLight_.direction.x, -1.0f, 1.0f);
	ImGui::End();

}

void TitleScene::ChangeBackTexture(std::unique_ptr<BaseTitleBackTexture> backTexture){
	//違った時だけ遷移する
	if (baseTitleBackTexture_ != backTexture) {
		baseTitleBackTexture_ = std::move(backTexture);
		//引数が次に遷移する
		baseTitleBackTexture_->Initialize();
	}
}
