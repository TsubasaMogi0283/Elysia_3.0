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

	//タイトルに戻る
	isChangeNextScene_ = false;
}

void LoseScene::Update(Ellysia::GameManager* gameManager){

	//始め
	//ライトアップをする
	if (isFinishLightUp_ == false) {
		

		//ライトアップ終了する
		if (lightRadiusT_ > MAX_T_VALUE_) {
			//増える間隔
			const float INTERVAL = 0.025f;
			//不透明にしていく
			transparencyT_ += INTERVAL;
			textTransparency_ = Easing::EaseInQuart(transparencyT_);
			levelDataManager_->SetTransparency(levelDataHandle_, TO_GAME, textTransparency_);
			levelDataManager_->SetTransparency(levelDataHandle_, TO_TITLE, textTransparency_);

			//少しだけ待ってから選択できるようにする
			const float ADD_T_VALUE = 1.0f;
			if (transparencyT_ >= MAX_T_VALUE_+ ADD_T_VALUE) {
				displayText_ = true;
				isFinishLightUp_ = true;
			}

		}
		else {
			//増える間隔
			const float INTERVAL = 0.008f;
			lightRadiusT_ += INTERVAL;
			//点光源の半径を設定
			pointLight_.radius_ = Easing::EaseOutSine(lightRadiusT_) * MAX_LIGHT_RADIUS_;
			//テキストは非表示にする
			levelDataManager_->SetTransparency(levelDataHandle_, TO_GAME, 0.0f);
			levelDataManager_->SetTransparency(levelDataHandle_, TO_TITLE, 0.0f);
			//矢印も非表示にしておく
			levelDataManager_->SetInvisible(levelDataHandle_, SELECT_ARROW, true);

		}
	}
	else {

		if (displayText_ == true) {
			//ゲームかタイトルか選択する
			Select();

			//次のシーンへ変わる
			if (isChangeNextScene_ == true) {
				ChangeNextScene();
			}



			const float MOVE_FINISH_TIME = 3.0f;
			//ゲームへ
			if (cameraMoveTime_ > MOVE_FINISH_TIME) {
				gameManager->ChangeScene("Game");
				return;
			}
			// タイトルへ
			if (blackOutTime_ > CHANGE_TO_TITLE_TIME_) {
				gameManager->ChangeScene("Title");
				return;
			}
		}
		
	}

	
	//レベルデータの更新
	levelDataManager_->Update(levelDataHandle_);

	//カメラの更新
	camera_.translate = VectorCalculation::Add(camera_.translate, cameraVelocity_);
	camera_.Update();
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
	
	//再読み込み
	if (input_->IsTriggerKey(DIK_L) == true) {
		levelDataManager_->Reload(levelDataHandle_);
	}

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

	//フェードイン
	black_->Draw();
}

void LoseScene::Select() {

	//矢印を表示させる
	levelDataManager_->SetInvisible(levelDataHandle_, SELECT_ARROW, false);

	//矢印の回転
	const float ROTATE_VALUE = 0.1f;
	arrowRotate_ += ROTATE_VALUE;
	levelDataManager_->SetRotate(levelDataHandle_, SELECT_ARROW, { .x = 0.0f,.y = arrowRotate_ ,.z = 0.0f });

	//矢印の初期座標を取得
	Vector3 arrowInitialPosition = levelDataManager_->GetInitialTranslate(levelDataHandle_, SELECT_ARROW);
	//「ゲームへ」の座標を取得
	Vector3 toGameInitialPosition = levelDataManager_->GetInitialTranslate(levelDataHandle_, TO_GAME);
	//「タイトルへ」の座標を取得
	Vector3 toTitleInitialPosition = levelDataManager_->GetInitialTranslate(levelDataHandle_, TO_TITLE);
	
	//非選択時
	Vector3 noSelectedScale = levelDataManager_->GetInitiaScale(levelDataHandle_, TO_TITLE);
	//選択時の高さ
	float selectedHeight = 1.6f;

	//「ゲームへ」選択時
	if (isSelectingGame_ == true && isSelectingTitle_ == false) {

		//選択されている方である「ゲーム」は大きくする
		levelDataManager_->SetScale(levelDataHandle_, TO_GAME, { .x = selectedScale_,.y = selectedScale_ ,.z = selectedScale_ });
		levelDataManager_->SetTranslate(levelDataHandle_, TO_GAME, { .x = toGameInitialPosition.x,.y = selectedHeight ,.z = toGameInitialPosition.x });

		//選択されていない方である「タイトル」は初期位置で
		levelDataManager_->SetScale(levelDataHandle_, TO_TITLE, noSelectedScale);
		levelDataManager_->SetTranslate(levelDataHandle_, TO_TITLE, toTitleInitialPosition);

		//右・D押したとき「タイトルへ」に移動
		if ((input_->IsTriggerKey(DIK_RIGHT) == true) || (input_->IsTriggerKey(DIK_D) == true)) {
			levelDataManager_->SetTranslate(levelDataHandle_, SELECT_ARROW, { .x = toTitleInitialPosition.x,.y = arrowInitialPosition.y ,.z = toTitleInitialPosition.z });
			isSelectingGame_ = false;
			isSelectingTitle_ = true;
		}

	}
	//タイトルへ
	else if (isSelectingGame_ == false && isSelectingTitle_ == true) {

		//選択されている方である「タイトル」は大きくする
		levelDataManager_->SetScale(levelDataHandle_, TO_TITLE, { .x = selectedScale_,.y = selectedScale_ ,.z = selectedScale_ });
		levelDataManager_->SetTranslate(levelDataHandle_, TO_TITLE, { .x = toTitleInitialPosition.x,.y = selectedHeight ,.z = toTitleInitialPosition.z });

		//選択されていない方である「ゲーム」は初期位置で
		levelDataManager_->SetScale(levelDataHandle_, TO_GAME, noSelectedScale);
		levelDataManager_->SetTranslate(levelDataHandle_, TO_GAME, toGameInitialPosition);
		//左・A押したとき「タイトルへ」に移動
		if ((input_->IsTriggerKey(DIK_LEFT) == true) || (input_->IsTriggerKey(DIK_A) == true)) {
			levelDataManager_->SetTranslate(levelDataHandle_, SELECT_ARROW, { .x = toGameInitialPosition.x,.y = arrowInitialPosition.y ,.z = toGameInitialPosition.z });
			isSelectingGame_ = true;
			isSelectingTitle_ = false;
		}
	}

	//コントローラー接続時
	if (input_->IsConnetGamePad() == true) {

		//Bボタンを押したとき
		if (input_->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_B) {
			bTriggerTime_ += INCREASE_VALUE_;

		}
		if ((input_->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_B) == 0) {
			//Bトリガーの反応しない時間
			const uint32_t NO_REACT_TIME = 0u;
			bTriggerTime_ = NO_REACT_TIME;
		}

		//Bトリガーの反応する時間
		const uint32_t REACT_TIME = 1u;
		if (bTriggerTime_ == REACT_TIME) {
			isChangeNextScene_ = true;
		}
	}
	//次のシーンへ
	if (input_->IsTriggerKey(DIK_SPACE) == true) {
		isChangeNextScene_ = true;
	}
}

void LoseScene::ChangeNextScene(){
	//タイトルへ戻る
	if (isSelectingGame_ == false && isSelectingTitle_ == true) {

		//矢印の回転
		const float ROTATE_VALUE = 0.1f;
		arrowRotate_ += ROTATE_VALUE;
		levelDataManager_->SetRotate(levelDataHandle_, SELECT_ARROW, { .x = 0.0f,.y = arrowRotate_ ,.z = 0.0f });

		arrowDropT_ += 0.05f;
		const float TROTATE_AMOUNT = 0.5f;
		float newT = (1.0f - Easing::EaseOutQuart(arrowDropT_))* TROTATE_AMOUNT;


		levelDataManager_->SetScale(levelDataHandle_, SELECT_ARROW, { .x = newT* selectedScale_,.y = newT* selectedScale_ ,.z = newT* selectedScale_ });

		//指定した時間を超えたらタイトルへ遷移
		if (fastFlashTime_ > FAST_FLASH_TIME_LIMIT_) {
			returnToTitleDissolveThresholdT_ += 0.01f;
			//ディゾルブの値を増やす
			dissolve_.threshold = Easing::EaseInSine(returnToTitleDissolveThresholdT_);
			//暗くなったらシーンチェンジ
			transparency_ += TRANSPARENCY_INTERVAL_;
		}

		if (transparency_ > MAX_T_VALUE_) {
			blackOutTime_ += INCREASE_VALUE_;
		}
	}
	//もう一度挑戦
	else if (isSelectingGame_ == true && isSelectingTitle_ == false) {

		//矢印の初期座標を取得
		Vector3 arrowInitialPosition = levelDataManager_->GetInitialTranslate(levelDataHandle_, SELECT_ARROW);

		//決定されたら跳ねるような動きをし、回転が止まる
		const float INTERVAL = 0.1f;
		decideArrowMoveTheta_ += INTERVAL;
		float newTheta = std::clamp(decideArrowMoveTheta_, 0.0f, std::numbers::pi_v<float>);
		
		//高さ
		const float HEIGHT = 1.0f;
		float newArrowPositionY = arrowInitialPosition.y + std::sinf(newTheta) * HEIGHT;

		levelDataManager_->SetTranslate(levelDataHandle_, SELECT_ARROW, { .x = arrowInitialPosition.x,.y = newArrowPositionY ,.z = arrowInitialPosition.z });

		//点滅
		fastFlashTime_ += INCREASE_VALUE_;

		//指定した時間を超えたらタイトルへ遷移
		if (fastFlashTime_ > FAST_FLASH_TIME_LIMIT_) {
			//加速
			Vector3 cameraAccelation_ = { .x = 0.0f,.y = 0.001f,.z = -0.01f };
			cameraVelocity_ = VectorCalculation::Add(cameraVelocity_, cameraAccelation_);
			//時間
			cameraMoveTime_ += DELTA_TIME;
		}
	}

}

void LoseScene::DisplayImGui(){

	ImGui::Begin("脱出失敗シーン");

	if (ImGui::TreeNode("点光源")) {
		ImGui::InputFloat("T", &lightRadiusT_);
		ImGui::SliderFloat3("座標", &pointLight_.position_.x, -40.0f, 40.0f);
		ImGui::SliderFloat("Decay", &pointLight_.decay_, 0.0f, 20.0f);
		ImGui::SliderFloat("半径", &pointLight_.radius_, 0.0f, 20.0f);
		ImGui::TreePop();
		
	}
	if (ImGui::TreeNode("ディゾルブ")) {
		ImGui::SliderFloat("しきい値", &dissolve_.threshold, 0.0f, 2.0f);
		ImGui::SliderFloat("厚さ", &dissolve_.edgeThinkness, 0.0f, 2.0f);
		ImGui::TreePop();
		
	}
	if (ImGui::TreeNode("テキスト")) {
		ImGui::InputFloat("textTransparency_", &textTransparency_);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("カメラ")) {
		ImGui::SliderFloat3("座標", &camera_.translate.x, -40.0f, 40.0f);
		ImGui::TreePop();
	}

	ImGui::End();
	

}

void LoseScene::Adjustment(){
	globalVariables_->SaveFile(DISSOLVE_NAME_);
	globalVariables_->SaveFile(POINT_LIGHT_NAME);

}

