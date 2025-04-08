#include "LoseScene.h"

#include <imgui.h>

#include "Input.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "LevelDataManager.h"
#include "GameManager.h"
#include "VectorCalculation.h"
#include "GlobalVariables.h"
#include "Easing.h"
#include "BaseLoseScene/LightUp/LightUpLoseScene.h"


LoseScene::LoseScene(){

	//インスタンスの取得
	//入力クラス
	input_ = Elysia::Input::GetInstance();
	//テクスチャ管理クラス
	textureManager_ = Elysia::TextureManager::GetInstance();
	//レベルデータ管理クラス
	levelDataManager_ = Elysia::LevelDataManager::GetInstance();
	//モデル管理クラス
	modelManager_ = Elysia::ModelManager::GetInstance();
	//グローバル変数クラス
	globalVariables_ = Elysia::GlobalVariables::GetInstance();
}

void LoseScene::Initialize(){

	//スプライトの初期座標
	const Vector2 INITIAL_SPRITE_POSITION_ = { .x = 0.0f,.y = 0.0f };

	//黒背景
	uint32_t blackTextureHandle = textureManager_->Load("Resources/Sprite/Back/Black.png");
	black_.reset(Elysia::Sprite::Create(blackTextureHandle, INITIAL_SPRITE_POSITION_));
	//透明度の設定
	transparency_ = 0.0f;
	black_->SetTransparency(transparency_);
	blackOutTime_ = 0u;
	
	//負けシーン用のレベルデータを入れる
	levelDataHandle_ = levelDataManager_->Load("LoseStage/LoseStage.json");
	

	//背景(ポストエフェクト)
	backTexture_ = std::make_unique<Elysia::BackTexture>();
	const Vector4 CLEAR_COLOR = { .x = 0.0f,.y = 0.0f,.z = 0.0f,.w = 1.0f };
	backTexture_->SetClearColour(CLEAR_COLOR);
	backTexture_->Initialize();

	//ディゾルブ
	dissolveEffect_ = std::make_unique<Elysia::DissolvePostEffect>();
	dissolveEffect_->Initialize(CLEAR_COLOR);
	//マスクテクスチャ
	uint32_t maskTexture = Elysia::TextureManager::GetInstance()->Load("Resources/External/Texture/Dissolve/noise0.png");


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
	globalVariables_->CreateGroup(POINT_LIGHT_NAME_);
	globalVariables_->AddItem(POINT_LIGHT_NAME_, "Translate", pointLight_.position);
	globalVariables_->AddItem(POINT_LIGHT_NAME_, "Decay", pointLight_.decay);

	//初期化
	pointLight_.Initialize();
	pointLight_.position = globalVariables_->GetVector3Value(POINT_LIGHT_NAME_, "Translate");
	pointLight_.decay = globalVariables_->GetFloatValue(POINT_LIGHT_NAME_,"Decay");
	pointLight_.radius = 0.0f;

	//タイトルに戻る
	isChangeNextScene_ = false;

	//細かいシーン
	detailLoseScene_ = std::make_unique<LightUpLoseScene>();
	//レベルデータハンドルの設定
	detailLoseScene_->SetLevelDataHandle(levelDataHandle_);
	//初期化
	detailLoseScene_->Initialize();
	

}

void LoseScene::Update(Elysia::GameManager* gameManager){

	//細かいシーンの更新
	detailLoseScene_->Update(this);

	//始め
	//ライトアップをする
	if (isFinishLightUp_ == false) {
		

		////ライトアップ終了する
		//if (startLightUpT_ > MAX_T_VALUE_) {
		//	//増える間隔
		//	const float_t INTERVAL = 0.025f;
		//	//不透明にしていく
		//	transparencyT_ += INTERVAL;
		//	textTransparency_ = Easing::EaseInQuart(transparencyT_);
		//	levelDataManager_->SetTransparency(levelDataHandle_, TO_GAME, textTransparency_);
		//	levelDataManager_->SetTransparency(levelDataHandle_, TO_TITLE, textTransparency_);
		//
		//	//少しだけ待ってから選択できるようにする
		//	const float_t ADD_T_VALUE = 1.0f;
		//	if (transparencyT_ >= MAX_T_VALUE_+ ADD_T_VALUE) {
		//		displayText_ = true;
		//		isFinishLightUp_ = true;
		//	}
		//
		//}
		//else {
		//	//増える間隔
		//	const float_t INTERVAL = 0.008f;
		//	startLightUpT_ += INTERVAL;
		//	//点光源の半径を設定
		//	pointLight_.radius = Easing::EaseOutSine(startLightUpT_) * MAX_LIGHT_RADIUS_;
		//	//テキストは非表示にする
		//	levelDataManager_->SetTransparency(levelDataHandle_, TO_GAME, 0.0f);
		//	levelDataManager_->SetTransparency(levelDataHandle_, TO_TITLE, 0.0f);
		//	//矢印も非表示にしておく
		//	levelDataManager_->SetInvisible(levelDataHandle_, SELECT_ARROW, true);
		//
		//}
	}
	else {

		//if (displayText_ == true) {
		//	//ゲームかタイトルか選択する
		//	if (isChangeNextScene_ == false) {
		//		Select();
		//	}
		//	//次のシーンへ変わる
		//	else {
		//		ChangeNextScene();
		//	}
		//	
		//
		//	//ゲームへ
		//	if (pointLight_.radius <= 0.0f) {
		//		gameManager->ChangeScene("Game");
		//		return;
		//	}
		//	// タイトルへ
		//	if (blackOutTime_ > CHANGE_TO_TITLE_TIME_) {
		//		gameManager->ChangeScene("Title");
		//		return;
		//	}
		//}
		
	}

	gameManager;
	//レベルデータの更新
	levelDataManager_->Update(levelDataHandle_);

	//カメラの更新
	camera_.translate = VectorCalculation::Add(camera_.translate, detailLoseScene_->GetCameraVelocity());
	camera_.Update();
	//点光源の更新
	pointLight_.position = globalVariables_->GetVector3Value(POINT_LIGHT_NAME_, "Translate");
	pointLight_.decay = globalVariables_->GetFloatValue(POINT_LIGHT_NAME_, "Decay");
	pointLight_.radius = detailLoseScene_->GetPointLight().radius;
	pointLight_.Update();
	//ディゾルブの更新
	dissolve_.threshold = detailLoseScene_->GetDissolve().threshold;
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

void LoseScene::PreDrawPostEffect(){
	dissolveEffect_->PreDraw();
}

void LoseScene::DrawPostEffect(){
	dissolveEffect_->Draw(dissolve_);
}

void LoseScene::DrawSprite(){

	//フェードイン
	black_->Draw();
}

void LoseScene::ChangeDetailScene(std::unique_ptr<BaseLoseScene>detailScene){
	//違った時だけ遷移する
	if (detailLoseScene_ != detailScene) {
		detailLoseScene_ = std::move(detailScene);
		//次に遷移する
		detailLoseScene_->SetLevelDataHandle(levelDataHandle_);
		detailLoseScene_->Initialize();
	}
}

void LoseScene::Select() {

	//矢印を表示させる
	levelDataManager_->SetInvisible(levelDataHandle_, SELECT_ARROW, false);

	//矢印の回転
	arrowRotate_ += ROTATE_VALUE_;
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
		if ((input_->IsTriggerKey(DIK_RIGHT) == true) || (input_->IsTriggerKey(DIK_D) == true )||(input_->IsTriggerButton(XINPUT_GAMEPAD_DPAD_RIGHT)==true)) {
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
		if ((input_->IsTriggerKey(DIK_LEFT) == true) || (input_->IsTriggerKey(DIK_A) == true)|| input_->IsTriggerButton(XINPUT_GAMEPAD_DPAD_LEFT) == true) {
			levelDataManager_->SetTranslate(levelDataHandle_, SELECT_ARROW, { .x = toGameInitialPosition.x,.y = arrowInitialPosition.y ,.z = toGameInitialPosition.z });
			isSelectingGame_ = true;
			isSelectingTitle_ = false;
		}
	}

	//Bボタン、スペースキーを押したとき
	if (input_->IsTriggerButton(XINPUT_GAMEPAD_B) == true|| input_->IsTriggerKey(DIK_SPACE) == true) {
		isChangeNextScene_ = true;
	}
}

void LoseScene::ChangeNextScene(){

	//終わる時間
	const uint32_t FINISH_WAIT_TIME = 60u;

	//タイトルへ戻る
	if (isSelectingGame_ == false && isSelectingTitle_ == true) {

		//矢印の初期座標を取得
		Vector3 arrowInitialPosition = levelDataManager_->GetInitialTranslate(levelDataHandle_, SELECT_ARROW);
		//「タイトルへ」の座標を取得
		Vector3 toTitleInitialPosition = levelDataManager_->GetInitialTranslate(levelDataHandle_, TO_TITLE);

		//決定されたら跳ねるような動きをする
		const float INTERVAL = 0.1f;
		decideArrowMoveTheta_ += INTERVAL;
		float newTheta = std::clamp(decideArrowMoveTheta_, 0.0f, std::numbers::pi_v<float>);
		if (decideArrowMoveTheta_ < std::numbers::pi_v<float>) {
			//高速回転
			const float FAST_ROTATE_VALUE = 0.5f;
			arrowRotate_ += FAST_ROTATE_VALUE;
			//新しい回転を設定
			levelDataManager_->SetRotate(levelDataHandle_, SELECT_ARROW, { .x = 0.0f,.y = arrowRotate_ ,.z = 0.0f });
		}
		else {
			//回転を止める
			levelDataManager_->SetRotate(levelDataHandle_, SELECT_ARROW, { .x = 0.0f,.y = std::numbers::pi_v <float> / 2.0f ,.z = 0.0f });
		}


		//高さ
		const float HEIGHT = 1.0f;
		float newArrowPositionY = arrowInitialPosition.y + std::sinf(newTheta) * HEIGHT;
		//新しい座標を設定
		levelDataManager_->SetTranslate(levelDataHandle_, SELECT_ARROW, { .x = toTitleInitialPosition.x,.y = newArrowPositionY ,.z = arrowInitialPosition.z });

		//カメラの動きを待つ時間
		waitForCameraMoveTime_ += INCREASE_VALUE_;

		//指定した時間を超えたらタイトルへ遷移
		if (waitForCameraMoveTime_ > FINISH_WAIT_TIME) {
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

		//決定されたら跳ねるような動きをする
		const float INTERVAL = 0.1f;
		decideArrowMoveTheta_ += INTERVAL;
		float newTheta = std::clamp(decideArrowMoveTheta_, 0.0f, std::numbers::pi_v<float>);
		if (decideArrowMoveTheta_ < std::numbers::pi_v<float>) {
			//高速回転
			const float FAST_ROTATE_VALUE = 0.5f;
			arrowRotate_ += FAST_ROTATE_VALUE;
			//新しい回転を設定
			levelDataManager_->SetRotate(levelDataHandle_, SELECT_ARROW, { .x = 0.0f,.y = arrowRotate_ ,.z = 0.0f });
		}
		else {
			//回転を止める
			levelDataManager_->SetRotate(levelDataHandle_, SELECT_ARROW, { .x = 0.0f,.y = std::numbers::pi_v <float>/2.0f ,.z = 0.0f });
		}
		
		
		//高さ
		const float HEIGHT = 1.0f;
		float newArrowPositionY = arrowInitialPosition.y + std::sinf(newTheta) * HEIGHT;
		//新しい座標を設定
		levelDataManager_->SetTranslate(levelDataHandle_, SELECT_ARROW, { .x = arrowInitialPosition.x,.y = newArrowPositionY ,.z = arrowInitialPosition.z });

		//カメラの動きを待つ時間
		waitForCameraMoveTime_ += INCREASE_VALUE_;

		//指定した時間を超えたらゲームへ遷移
		if (waitForCameraMoveTime_ > FINISH_WAIT_TIME) {
			//ライトの半径を小さくしていく
			const float NARROW_VALUE = 0.002f;
			endLightUpT_ += NARROW_VALUE;
			pointLight_.radius = std::clamp((1.0f - Easing::EaseOutSine(endLightUpT_)),0.0f,1.0f) * MAX_LIGHT_RADIUS_;

			//加速
			Vector3 cameraAccelation_ = { .x = 0.0f,.y = 0.001f,.z = -0.01f };
			cameraVelocity_ = VectorCalculation::Add(cameraVelocity_, cameraAccelation_);
		}
	}

}

void LoseScene::DisplayImGui(){

	ImGui::Begin("脱出失敗シーン");

	if (ImGui::TreeNode("点光源")) {
		ImGui::InputFloat("T", &startLightUpT_);
		ImGui::SliderFloat3("座標", &pointLight_.position.x, -40.0f, 40.0f);
		ImGui::SliderFloat("Decay", &pointLight_.decay, 0.0f, 20.0f);
		ImGui::SliderFloat("半径", &pointLight_.radius, 0.0f, 20.0f);
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
	globalVariables_->SaveFile(POINT_LIGHT_NAME_);

}

