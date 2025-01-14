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
#include <VectorCalculation.h>
#include <SingleCalculation.h>

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
	//黒フェード
	uint32_t blackTexureHandle= textureManager_->LoadTexture("Resources/Sprite/Back/Black.png");

	//初期座標
	const Vector2 INITIAL_POSITION = {.x=0.0f,.y=0.0f};
	
	//テキスト
	text_.reset(Sprite::Create(logoTextureHandle, INITIAL_POSITION));
	//ロゴ
	backGround_.reset(Sprite::Create(titleTextureHandle, INITIAL_POSITION));
	//黒フェード
	blackFade_.reset(Sprite::Create(blackTexureHandle, INITIAL_POSITION));
	//初期の透明度設定
	const float INITIAL_TRANSPARENCY = 0.0f;
	blackFade_->SetTransparency(INITIAL_TRANSPARENCY);


	levelHandle_ = levelDataManager_->Load("TitleStage/TitleStage.json");

	isStart_ = false;
	isFlash_ = true;
	isFastFlash_ = false;

	//マテリアルの初期化
	material_.Initialize();
	material_.lightingKinds_ = Directional;

	//スポットライトの初期化
	spotLight.Initialize();

	directionalLight_.Initialize();

	//カメラの初期化
	camera_.Initialize();
	//座標
	camera_.translate = {.x = 0.0f,.y = 0.0f,.z = -9.8f };

	//レールカメラ
	titleRailCamera_ = std::make_unique<TitleRailCamera>();
	//初期化
	titleRailCamera_->Initialize();
	

	//ポストエフェクト
	back_ = std::make_unique<BackText>();
	//初期化
	back_->Initialize();


	//ランダムエフェクトの生成
	randomEffect_ = std::make_unique<RandomEffect>();
	//初期化
	randomEffect_->Initialize();
	

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
	
#pragma region スタート演出
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
	if (isFastFlash_ == true&& isStart_==false) {
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


#pragma endregion


	//シーン遷移演出
	if (isStart_ == true) {

		text_->SetInvisible(false);

		//時間の加算
		const int32_t DELTA_TIME = 1u;
		randomEffectTime_ += DELTA_TIME;

		//開始時間
		const int32_t RANDOM_EFFECT_DISPLAY_START_TIME_[DISPLAY_LENGTH_QUANTITY_] = {00u,140u};
		//表示の長さ
		const int32_t RANDOM_EFFECT_DISPLAY_LENGTH_[DISPLAY_LENGTH_QUANTITY_] = { 60u,180u };


#ifdef _DEBUG
		ImGui::Begin("TitleFade&Effect");
		ImGui::InputInt("Count", &randomEffectTime_);
		ImGui::Checkbox("IsDisplay", &isDisplayRandomEffect_);
		ImGui::End();
#endif // _DEBUG


		for (uint32_t i = 0; i < DISPLAY_LENGTH_QUANTITY_; ++i) {
			
		}
		

		if (randomEffectTime_ > RANDOM_EFFECT_DISPLAY_START_TIME_[0] &&
			randomEffectTime_ <= RANDOM_EFFECT_DISPLAY_START_TIME_[0] + RANDOM_EFFECT_DISPLAY_LENGTH_[0]) {
			isDisplayRandomEffect_ = true;
		}
		else if (randomEffectTime_ > RANDOM_EFFECT_DISPLAY_START_TIME_[1] &&
			randomEffectTime_ <= RANDOM_EFFECT_DISPLAY_START_TIME_[1] + RANDOM_EFFECT_DISPLAY_LENGTH_[1]) {
			isDisplayRandomEffect_ = true;
		}
		else {
			isDisplayRandomEffect_ = false;
		}



		//ランダムの終了
		if (randomEffectTime_ > RANDOM_EFFECT_DISPLAY_START_TIME_[1] + RANDOM_EFFECT_DISPLAY_LENGTH_[1]) {
			isEndDisplayRandomEffect_ = true;
		}
		
		if (isEndDisplayRandomEffect_ == true) {
			const float FADE_INCREASE_VALUE = 0.01f;
			blackFadeTransparency_ += FADE_INCREASE_VALUE;
			
		}


		if (blackFadeTransparency_ > 2.0f) {
			gameManager->ChangeScene(new GameScene());
			return;
		}
		
	}



	blackFade_->SetTransparency(blackFadeTransparency_);

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
	directionalLight_.Update();

	//レールカメラの更新
	titleRailCamera_->Update();
	camera_.viewMatrix = titleRailCamera_->GetCamera().viewMatrix;
	camera_.projectionMatrix = titleRailCamera_->GetCamera().projectionMatrix;

	//カメラの更新
	camera_.Transfer();




}

void TitleScene::DrawObject3D(){
	//ステージオブジェクト
	levelDataManager_->Draw(levelHandle_,camera_, material_, directionalLight_);


}

void TitleScene::PreDrawPostEffectFirst(){
	if (isDisplayRandomEffect_ == false) {
		back_->PreDraw();

	}
	else {
		//ランダム
		randomEffect_->PreDraw();
	}

	
	
}

void TitleScene::DrawPostEffect(){
	if (isDisplayRandomEffect_ == false) {
		back_->Draw();
	}
	else {
		//ランダムエフェクト
		randomEffect_->Draw();

	}
	
}

void TitleScene::DrawSprite(){
	//背景
	//backGround_->Draw();

	//テキスト
	text_->Draw();
	
	//黒フェード
	blackFade_->Draw();

}
