#include "LoseScene.h"

#include "Input.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "LevelDataManager.h"

#include "GameManager.h"
#include "VectorCalculation.h"
#include "GlobalVariables.h"
#include "Easing.h"



LoseScene::LoseScene(){

	//インスタンスの取得
	//入力クラス
	input_ = Ellysia::Input::GetInstance();
	//テクスチャ管理クラス
	textureManager_ = Ellysia::TextureManager::GetInstance();
	//レベルデータ管理クラス
	levelDataManager_ = Ellysia::LevelDataManager::GetInstance();
	//モデル管理クラス
	modelManager_ = Ellysia::ModelManager::GetInstance();
	//グローバル変数クラス
	globalVariables_ = Ellysia::GlobalVariables::GetInstance();
}

void LoseScene::Initialize(){

	//スプライトの初期座標
	const Vector2 INITIAL_SPRITE_POSITION_ = { .x = 0.0f,.y = 0.0f };

	//Text
	uint32_t textHandle = textureManager_->LoadTexture("Resources/Sprite/Result/Lose/LoseText.png");
	text_.reset(Ellysia::Sprite::Create(textHandle, INITIAL_SPRITE_POSITION_));

	//黒背景
	uint32_t blackTextureHandle = textureManager_->LoadTexture("Resources/Sprite/Back/Black.png");
	black_.reset(Ellysia::Sprite::Create(blackTextureHandle, INITIAL_SPRITE_POSITION_));
	//透明度の設定
	transparency_ = 0.0f;
	black_->SetTransparency(transparency_);
	blackOutTime_ = 0u;
	
	//負けシーン用のレベルデータを入れる
	levelDataHandle_ = levelDataManager_->Load("LoseStage/LoseStage.json");
	

	//背景(ポストエフェクト)
	backTexture_ = std::make_unique<Ellysia::BackTexture>();
	const Vector4 CLEAR_COLOR = { .x = 0.0f,.y = 0.0f,.z = 0.0f,.w = 1.0f };
	backTexture_->SetClearColour(CLEAR_COLOR);
	backTexture_->Initialize();

	//ディゾルブ
	dissolveEffect_ = std::make_unique<Ellysia::DissolvePostEffect>();
	dissolveEffect_->Initialize(CLEAR_COLOR);
	//マスクテクスチャ
	uint32_t maskTexture = Ellysia::TextureManager::GetInstance()->LoadTexture("Resources/External/Texture/Dissolve/noise0.png");


	//調整項目として記録
	globalVariables_->CreateGroup(DISSOLVE_NAME_);
	globalVariables_->AddItem(DISSOLVE_NAME_, "Thinkness", dissolve_.edgeThinkness);


	//初期化
	dissolve_.Initialize();
	dissolve_.maskTextureHandle = maskTexture;
	dissolve_.edgeThinkness = globalVariables_->GetFloatValue(DISSOLVE_NAME_, "Thinkness");
	dissolve_.threshold = 0.0f;
	//カメラの初期化
	camera_.Initialize();
	camera_.translate = { .x = 0.0f,.y = 2.8f,.z = -23.0f };
	//マテリアル
	material_.Initialize();
	material_.lightingKinds_ = PointLighting;

	//点光源
	//調整項目として記録
	globalVariables_->CreateGroup(POINT_LIGHT_NAME);
	globalVariables_->AddItem(POINT_LIGHT_NAME, "Translate", pointLight_.position_);
	globalVariables_->AddItem(POINT_LIGHT_NAME, "Decay", pointLight_.decay_);

	//初期化
	pointLight_.Initialize();
	pointLight_.position_ = globalVariables_->GetVector3Value(POINT_LIGHT_NAME, "Translate");
	pointLight_.decay_ = globalVariables_->GetFloatValue(POINT_LIGHT_NAME,"Decay");
	pointLight_.radius_ = 0.0f;

	//点滅
	isFlash_ = true;

	//タイトルに戻る
	isReturnTitle = false;
}

void LoseScene::Update(Ellysia::GameManager* gameManager){

	//増える時間の値
	const uint32_t INCREASE_VALUE = 1u;
	
	//始め
	//ライトアップをする
	if (isFinishLightUp_ == false) {
		//増える間隔
		const float INTERVAL = 0.008f;
		lightRadiusT_ += INTERVAL;
		pointLight_.radius_=Easing::EaseOutSine(lightRadiusT_)* MAX_LIGHT_RADIUS_;

		//まだテキストは表示させない
		text_->SetInvisible(true);
		if (lightRadiusT_ > 1.0f) {
			isFinishLightUp_ = true;
		}
	}
	else {
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
			//再スタート時間
			const uint32_t RESTART_TIME = 0u;
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
				//Bトリガーの反応しない時間
				const uint32_t NO_REACT_TIME = 0u;
				bTriggerTime_ = NO_REACT_TIME;
			}

			//Bトリガーの反応する時間
			const uint32_t REACT_TIME = 1u;
			if (bTriggerTime_ == REACT_TIME) {

				isReturnTitle = true;
			}
		}
		//次のシーンへ
		if (input_->IsTriggerKey(DIK_SPACE) == true) {
			//ゲームへ
			isReturnToGame_ = true;

		}
		else if (input_->IsTriggerKey(DIK_T) == true) {
			//ゲームへ
			isReturnTitle = true;

		}
		


		//カウントが増える時間
		const uint32_t INCREASE_COUNT_TIME = 0u;
		//点滅の間隔
		const uint32_t FLASH_INTERVAL = 2u;
		//表示
		const uint32_t DISPLAY = 0u;

		//タイトルへ戻る
		if (isReturnTitle == true) {

			//高速点滅
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
				returnToTitleDissolveThresholdT_ += 0.01f;
				dissolve_.threshold = Easing::EaseInSine(returnToTitleDissolveThresholdT_);

				//テキストの非表示
				text_->SetInvisible(true);
				//透明度の設定
				//black_->SetTransparency(transparency_);
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
				Vector3 cameraAccelation_ = { .x = 0.0f,.y = 0.001f,.z = -0.01f };
				cameraVelocity_ = VectorCalculation::Add(cameraVelocity_, cameraAccelation_);

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
	}

	
	//レベルデータの更新
	levelDataManager_->Update(levelDataHandle_);


	//カメラの更新
	camera_.translate = VectorCalculation::Add(camera_.translate, cameraVelocity_);
	camera_.Update();
	//マテリアルの更新
	material_.Update();
	//点光源の更新
	pointLight_.position_ = globalVariables_->GetVector3Value(POINT_LIGHT_NAME, "Translate");
	pointLight_.decay_ = globalVariables_->GetFloatValue(POINT_LIGHT_NAME, "Decay");
	pointLight_.Update();
	//ディゾルブの更新
	dissolve_.edgeThinkness = globalVariables_->GetFloatValue(DISSOLVE_NAME_, "Thinkness");
	dissolve_.Update();


	//調整
	Adjustment();
	
#ifdef _DEBUG
	//ImGui
	DisplayImGui();
	
#endif // _DEBUG
	
	
}


void LoseScene::DrawObject3D(){
	//レベルデータ
	levelDataManager_->Draw(levelDataHandle_, camera_, pointLight_);
	
}

void LoseScene::PreDrawPostEffectFirst(){
	dissolveEffect_->PreDraw();
}

void LoseScene::DrawPostEffect(){
	dissolveEffect_->Draw(dissolve_);
}

void LoseScene::DrawSprite(){
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
	ImGui::InputFloat("T", &lightRadiusT_);
	ImGui::SliderFloat3("座標", &pointLight_.position_.x, -40.0f, 40.0f);
	ImGui::SliderFloat("Decay", &pointLight_.decay_, 0.0f, 20.0f);
	ImGui::SliderFloat("半径", &pointLight_.radius_, 0.0f, 20.0f);
	ImGui::End();

	ImGui::Begin("ディゾルブ");
	ImGui::SliderFloat("閾値", &dissolve_.threshold, 0.0f, 2.0f);
	ImGui::SliderFloat("厚さ", &dissolve_.edgeThinkness, 0.0f, 2.0f);
	
	ImGui::End();
}

void LoseScene::Adjustment(){
	globalVariables_->SaveFile(DISSOLVE_NAME_);
	globalVariables_->SaveFile(POINT_LIGHT_NAME);

}

