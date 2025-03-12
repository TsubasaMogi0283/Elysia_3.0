#include "WinScene.h"
#include "TextureManager.h"
#include "LevelDataManager.h"
#include "GameManager.h"
#include "Input.h"
#include <VectorCalculation.h>

WinScene::WinScene(){
	//インスタンスの取得
	//入力
	input_ = Ellysia::Input::GetInstance();
	//レベルデータ管理クラス
	levelDataManager_ = Ellysia::LevelDataManager::GetInstance();
}

void WinScene::Initialize() {
	//スプライトの初期位置
	const Vector2 INITIAL_SPRITE_POSITION = { .x = 0.0f,.y = 0.0f };


	//テキスト(タイトルへのやつ)
	uint32_t textHandle = Ellysia::TextureManager::GetInstance()->LoadTexture("Resources/Sprite/Result/Win/WinText.png");
	text_.reset(Ellysia::Sprite::Create(textHandle, INITIAL_SPRITE_POSITION));

	//白背景
	uint32_t whiteTexturehandle = Ellysia::TextureManager::GetInstance()->LoadTexture("Resources/Sprite/Back/White.png");
	//生成
	whiteFade_.reset(Ellysia::Sprite::Create(whiteTexturehandle, INITIAL_SPRITE_POSITION));
	//透明度の設定
	transparency_ = 0.0f;
	whiteFade_->SetTransparency(transparency_);
	
	//ブラックアウトの時間を初期化
	blackOutTime_ = 0;

	//レベルデータの読み込み
	levelDataHandle_ = levelDataManager_->Load("WinStage/WinStage.json");

	//マテリアルの初期化
	material_.Initialize();
	material_.lightingKinds = DirectionalLighting;

	//平行光源の初期化
	directionalLight_.Initialize();
	directionalLight_.color = { .x = 1.0f,.y = 0.22f,.z = 0.0f,.w = 1.0f };
	directionalLight_.direction = { .x = 0.91f,.y = -1.0f,.z = 0.0f };

	//カメラの初期化
	camera_.Initialize();
	camera_.translate = { .x = 0.0f,.y = 2.0f,.z = -6.0f };
	//ポストエフェクト
	backTexture_ = std::make_unique<Ellysia::BackTexture>();
	backTexture_->SetClearColour(directionalLight_.color);
	backTexture_->Initialize();

	//点滅
	isFlash_ = true;

	//再開
	restart_ = false;

}

void WinScene::Update(Ellysia::GameManager* gameManager){

	//増える時間の値
	const uint32_t INCREASE_VALUE = 1u;
	//再スタート時間
	const uint32_t RESTART_TIME = 0u;



	//通常の点滅
	flashTime_ += INCREASE_VALUE;
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
		flashTime_ = RESTART_TIME;
	}



	//カウントが増える時間
	const uint32_t INCREASE_COUNT_TIME = 0u;
	//点滅の間隔
	const uint32_t FLASH_INTERVAL = 2u;
	//表示
	const uint32_t DISPLAY = 0u;

	//次のシーンへ
	//Bボタンを押したとき
	if (input_->IsTriggerButton(XINPUT_GAMEPAD_B) == true) {
		restart_ = true;
	}
	//スペースを押したとき
	if (input_->IsTriggerKey(DIK_SPACE) == true) {
		restart_ = true;
	}

	//再スタート
	if (restart_ == true) {

		//時間を足していく
		fastFlashTime_ += INCREASE_VALUE;
		if (fastFlashTime_ % FAST_FLASH_TIME_INTERVAL_ == INCREASE_COUNT_TIME) {
			++textDisplayCount_;
		}
		//表示
		if (textDisplayCount_ % FLASH_INTERVAL == DISPLAY) {
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
			whiteFade_->SetTransparency(transparency_);

			//加速
			cameraAcceleration_.z = 0.05f;
			cameraVelocity_ = VectorCalculation::Add(cameraVelocity_, cameraAcceleration_);


			//暗くなったらシーンチェンジ
			transparency_ += TRANSPARENCY_INTERVAL;
		}

		//指定した時間まで時間が足される
		if (transparency_ > COMPLETELY_NO_TRANSPARENT_) {
			
			blackOutTime_ += INCREASE_VALUE;
		}
	}

	//タイトルシーンへ
	if (blackOutTime_ > CHANGE_TO_TITLE_TIME_) {
		gameManager->ChangeScene("Title");
		return;
	}

	

	//マテリアルの更新
	material_.Update();

	//平行光源の更新
	directionalLight_.Update();
	//カメラの更新
	camera_.translate = VectorCalculation::Add(camera_.translate, cameraVelocity_);
	camera_.Update();

#ifdef _DEBUG
	ImGui::Begin("カメラ"); 
	ImGui::SliderFloat3("座標", &camera_.translate.x, -30.0f, 30.0f);
	ImGui::End();
#endif // _DEBUG



	//脱出成功のオブジェクトの浮遊感を出すために
	//sinを使う
	const float FLLOATING_THETA_INTERVAL = 0.05f;
	objectFloatingTheta_ += FLLOATING_THETA_INTERVAL;
	//テキスト
	const std::string ESCAPE_SUCCEEDED_TEXT = "EscapeSucceededObject";
	//基準となる座標
	const float BASED_POSITION_Y = 6.0f;
	levelDataManager_->SetTranslate(levelDataHandle_, ESCAPE_SUCCEEDED_TEXT, {.x=0.0f,.y=std::sinf(objectFloatingTheta_) +BASED_POSITION_Y,.z=30.0f });


	//レベルデータの更新
	levelDataManager_->Update(levelDataHandle_);


}

void WinScene::PreDrawPostEffectFirst(){
	backTexture_->PreDraw();
}



void WinScene::DrawObject3D(){
	levelDataManager_->Draw(levelDataHandle_, camera_, directionalLight_);
}

void WinScene::DrawPostEffect(){
	backTexture_->Draw();
}

void WinScene::DrawSprite(){
	//テキスト
	text_->Draw();
	//フェードイン
	whiteFade_->Draw();


}

WinScene::~WinScene(){

}
