#include "LoseScene.h"

#include "Input.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "LevelDataManager.h"

#include "GameManager.h"
#include <VectorCalculation.h>



LoseScene::LoseScene(){

	//インスタンスの取得
	//入力クラス
	input_ = Ellysia::Input::GetInstance();
	//テクスチャ管理クラス
	textureManager_ = TextureManager::GetInstance();
	//レベルデータ管理クラス
	levelDataManager_ = Ellysia::LevelDataManager::GetInstance();
	//モデル管理クラス
	modelManager_ = ModelManager::GetInstance();

}

void LoseScene::Initialize(){

	//スプライトの初期座標
	const Vector2 INITIAL_SPRITE_POSITION_ = { .x = 0.0f,.y = 0.0f };

	//メイン
	uint32_t failedTextureHandle = textureManager_->LoadTexture("Resources/Sprite/Result/Lose/EscapeFailed.png");
	failedTexture_.reset(Sprite::Create(failedTextureHandle, INITIAL_SPRITE_POSITION_));

	//Text
	uint32_t textHandle = textureManager_->LoadTexture("Resources/Sprite/Result/Lose/LoseText.png");
	text_.reset(Sprite::Create(textHandle, INITIAL_SPRITE_POSITION_));

	//黒背景
	uint32_t blackTextureHandle = textureManager_->LoadTexture("Resources/Sprite/Back/Black.png");
	black_.reset(Sprite::Create(blackTextureHandle, INITIAL_SPRITE_POSITION_));
	//透明度の設定
	transparency_ = 0.0f;
	black_->SetTransparency(transparency_);
	blackOutTime_ = 0;
	
	//負けシーン用のレベルデータを入れる
	levelDataHandle_ = levelDataManager_->Load("LoseStage/LoseStage.json");
	

	//背景(ポストエフェクト)
	backTexture_ = std::make_unique<Ellysia::BackTexture>();
	const Vector4 CLEAR_COLOR = { .x = 0.0f,.y = 0.0f,.z = 0.0f,.w = 1.0f };
	backTexture_->SetClearColour(CLEAR_COLOR);
	backTexture_->Initialize();

	//カメラの初期化
	camera_.Initialize();
	camera_.translate = { .x = 0.0f,.y = 2.8f,.z = -28.3f };
	//マテリアル
	material_.Initialize();
	material_.lightingKinds_ = PointLighting;
	//点光源
	pointLight_.Initialize();
	pointLight_.position_ = { .x = 0.0f,.y = 6.1f,.z = -2.0f };
	pointLight_.decay_ = 3.0f;
	pointLight_.radius_ = 11.8f;

	//点滅
	isFlash_ = true;

	//タイトルに戻る
	isReturnTitle = false;
}

void LoseScene::Update(GameManager* gameManager){

	//増える時間の値
	const uint32_t INCREASE_VALUE = 1u;
	//Bトリガーの反応する時間
	const uint32_t REACT_TIME = 1u;
	//Bトリガーの反応しない時間
	const uint32_t NO_REACT_TIME = 0u;

	//再スタート時間
	const uint32_t RESTART_TIME = 0u;


	//通常の点滅
	flashTime_ += INCREASE_VALUE;
	if (flashTime_ > FLASH_TIME_LIMIT_ * 0 &&
		flashTime_ <= FLASH_TIME_LIMIT_) {
		text_->SetInvisible(false);
	}
	if (flashTime_ > FLASH_TIME_LIMIT_ &&
		flashTime_ <= FLASH_TIME_LIMIT_ * 2) {
		text_->SetInvisible(true);

	}
	if (flashTime_ > FLASH_TIME_LIMIT_ * 2) {
		flashTime_ = RESTART_TIME;
	}


	//ゲームかタイトルか選択する

	//コントローラー接続時
	if (input_->IsConnetGamePad() == true) {

		//Bボタンを押したとき
		if (input_->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_B) {
			bTriggerTime_ += INCREASE_VALUE;

		}
		if ((input_->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_B) == 0) {
			bTriggerTime_ = NO_REACT_TIME;
		}

		if (bTriggerTime_ == REACT_TIME) {

			isReturnTitle = true;
		}
	}
	//次のシーンへ
	if (input_->IsTriggerKey(DIK_SPACE) == true) {
		//ゲームへ
		isReturnToGame_ = true;

		//isReturnTitle = true;
	}


	//レベルデータの更新
	levelDataManager_->Update(levelDataHandle_);



	//カウントが増える時間
	const uint32_t INCREASE_COUNT_TIME = 0u;
	//点滅の間隔
	const uint32_t FLASH_INTERVAL = 2u;
	//表示
	const uint32_t DISPLAY = 0u;

	//タイトルへ戻る
	if (isReturnTitle == true) {

		//点滅
		fastFlashTime_ += INCREASE_VALUE;
		if (fastFlashTime_ % FAST_FLASH_TIME_INTERVAL_ == INCREASE_COUNT_TIME) {
			++textDisplayCount_;
		}
		//表示
		if (textDisplayCount_ % FLASH_INTERVAL == DISPLAY) {
			text_->SetInvisible(true);
		}
		else {
			text_->SetInvisible(false);
		}


		//指定した時間を超えたらタイトルへ遷移
		if (fastFlashTime_ > FAST_FLASH_TIME_LIMIT_) {
			//テキストの非表示
			text_->SetInvisible(true);
			//透明度の設定
			black_->SetTransparency(transparency_);
			//暗くなったらシーンチェンジ
			transparency_ += TRANSPARENCY_INTERVAL_;
		}

		const float MAX_TRANSPARENCY = 1.0f;
		if (transparency_ > MAX_TRANSPARENCY) {
			blackOutTime_ += INCREASE_VALUE;
		}


	}


	//ゲームは墓から遠ざかる
	if (isReturnToGame_ == true) {
		//点滅
		fastFlashTime_ += INCREASE_VALUE;
		
		//表示
		if (textDisplayCount_ % FLASH_INTERVAL == DISPLAY) {
			text_->SetInvisible(true);
		}
		else {
			text_->SetInvisible(false);
		}


		//指定した時間を超えたらタイトルへ遷移
		if (fastFlashTime_ > FAST_FLASH_TIME_LIMIT_) {
			//テキストの非表示
			text_->SetInvisible(true);
			
			//加速
			Vector3 cameraAccelation_ = {.x=0.0f,.y=0.001f,.z=-0.01f};
			cameraVelocity_ = VectorCalculation::Add(cameraVelocity_,cameraAccelation_);
		
			//時間
			cameraMoveTime_ += DELTA_TIME;
			

		}
	}


	const float MOVE_FINISH_TIME = 3.0f;
	if (cameraMoveTime_ > MOVE_FINISH_TIME) {
		gameManager->ChangeScene("Game");
		return;
	}




	//負けはビネット、タイトルへ
	if (blackOutTime_ > CHANGE_TO_TITLE_TIME_) {
		gameManager->ChangeScene("Title");
		return;
	}


	//カメラの更新
	camera_.translate = VectorCalculation::Add(camera_.translate, cameraVelocity_);
	camera_.Update();
	//マテリアルの更新
	material_.Update();
	//点光源の更新
	pointLight_.Update();

	
#ifdef _DEBUG
	//ImGui
	DisplayImGui();
#endif // _DEBUG

	
}


void LoseScene::DrawObject3D(){
	//レベルデータ
	levelDataManager_->Draw(levelDataHandle_, camera_, material_, pointLight_);
	
}

void LoseScene::PreDrawPostEffectFirst(){
	backTexture_->PreDraw();
}

void LoseScene::DrawPostEffect(){
	backTexture_->Draw();
}

void LoseScene::DrawSprite(){
	//負け
	//failedTexture_->Draw();
	//テキスト
	text_->Draw();
	//フェードイン
	black_->Draw();
}

void LoseScene::DisplayImGui(){


	ImGui::Begin("カメラ");
	ImGui::SliderFloat3("座標", &camera_.translate.x, -40.0f, 40.0f);
	ImGui::End();

	ImGui::Begin("点光源");
	ImGui::SliderFloat3("座標", &pointLight_.position_.x, -40.0f, 40.0f);
	ImGui::SliderFloat("Decay", &pointLight_.decay_, 0.0f, 20.0f);
	ImGui::SliderFloat("半径", &pointLight_.radius_, 0.0f, 20.0f);

	ImGui::End();


}

