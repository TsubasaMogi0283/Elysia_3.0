#include "GameScene.h"

#include <imgui.h>
#include <numbers>
#include <algorithm>

#include "Input.h"
#include "SingleCalculation.h"
#include "VectorCalculation.h"
#include "PushBackCalculation.h"
#include "AnimationManager.h"
#include "TextureManager.h"
#include "GameManager.h"
#include "ModelManager.h"
#include "LevelDataManager.h"
#include "GlobalVariables.h"

#include "BaseGameScene/Start/StartGameScene.h"
#include "BaseGameScene/Play/PlayGameScene.h"

GameScene::GameScene() {
	//インスタンスの取得
	//入力
	input_ = Elysia::Input::GetInstance();
	//テクスチャ管理クラス
	textureManager_ = Elysia::TextureManager::GetInstance();
	//モデル管理クラス
	modelManager_ = Elysia::ModelManager::GetInstance();
	//レベルエディタ管理クラス
	levelDataManager_ = Elysia::LevelDataManager::GetInstance();
	//グローバル変数クラス
	globalVariables_ = Elysia::GlobalVariables::GetInstance();
}

void GameScene::Initialize() {

	//ハンドルの取得
	levelHandle_ = levelDataManager_->Load("GameStage/GameStage.json");

	//生成
	player_ = std::make_unique<Player>();
	//初期化
	player_->Initialize();
	//ハンドルの設定
	player_->SetLevelHandle(levelHandle_);
	//最初はコントロールは出来ない用にする
	player_->SetIsAbleToControll(false);

	//鍵のモデルの読み込み
	uint32_t keyModelHandle = modelManager_->Load("Resources/External/Model/key", "Key.obj");
	//生成
	keyManager_ = std::make_unique<KeyManager>();
	//レベルデータハンドルの設定
	keyManager_->SetLevelDataHandle(levelHandle_);
	//プレイヤーの設定
	keyManager_->SetPlayer(player_.get());
	//レベルデータから鍵の情報を取り出す
	std::vector<Vector3> keyPositions = levelDataManager_->GetObjectPositions(levelHandle_,"Key");
	keyManager_->Initialize(keyModelHandle, keyPositions);

	//敵モデルの読み込み
	//通常
	uint32_t enemyModelHandle = modelManager_->Load("Resources/LevelData/GameStage/Ghost","Ghost.gltf",true);
	//強敵
	uint32_t strongEnemyModelHandle = modelManager_->Load("Resources/External/Model/01_HalloweenItems00/01_HalloweenItems00/EditedGLTF", "StrongGhost.gltf");
	//敵管理システム
	enemyManager_ = std::make_unique<EnemyManager>();
	//プレイヤーの設定
	enemyManager_->SetPlayer(player_.get());
	//レベルデータ管理クラスの設定
	enemyManager_->SetLevelDataManager(levelDataManager_, levelHandle_);
	//初期化
	//レベルデータから敵の情報を取り出す
	std::vector<Vector3> normalEnemyPositions = levelDataManager_->GetObjectPositions(levelHandle_, "NormalEnemy");
	std::vector<Vector3> strongEnemyPositions = levelDataManager_->GetObjectPositions(levelHandle_, "StrongEnemy");
	enemyManager_->Initialize(enemyModelHandle, strongEnemyModelHandle, normalEnemyPositions, strongEnemyPositions);

	//カメラの初期化
	camera_.Initialize();

	//ポストエフェクトの初期化
	//ビネット生成
	vignettePostEffect_ = std::make_unique<Elysia::VignettePostEffect>();
	//初期化
	vignettePostEffect_->Initialize();
	//値の設定
	vignette_.Initialize();
	vignette_.pow = 0.0f;

	//スタートから始まる
	detailGameScene_ = std::make_unique<StartGameScene>();

#ifdef _DEBUG
	//デバッグ時はプレイシーンから始める
	detailGameScene_ = std::make_unique<PlayGameScene>();

#endif // _DEBUG

	//レベルデータハンドルの設定
	detailGameScene_->SetLevelDataHandle(levelHandle_);
	//各ゲームオブジェクトのポインタを設定
	detailGameScene_->SetPlayer(player_.get());
	detailGameScene_->SetEnemyManager(enemyManager_.get());
	detailGameScene_->SetKeyManager(keyManager_.get());
	//初期化
	detailGameScene_->Initialize();

}


void GameScene::ChangeDetailScene(std::unique_ptr<BaseGameScene> detailGameScene){
	//違った時だけ遷移する
	if (detailGameScene_ != detailGameScene) {
		detailGameScene_ = std::move(detailGameScene);
		//レベルデータハンドルの設定
		detailGameScene_->SetLevelDataHandle(levelHandle_);
		//各ゲームオブジェクトのポインタを設定
		detailGameScene_->SetPlayer(player_.get());
		detailGameScene_->SetEnemyManager(enemyManager_.get());
		detailGameScene_->SetKeyManager(keyManager_.get());
		//次に遷移する
		detailGameScene_->Initialize();
	}
}

void GameScene::VigntteProcess(){
	//HPが1でピンチの場合
	const uint32_t DANGEROUS_HP = 1u;
	//プレイヤーがダメージを受けた場合ビネット
	if (player_->GetIsDamaged() == true) {
		//時間の加算
		vignetteChangeTime_ += DELTA_TIME_;

		//線形補間で滑らかに変化
		vignette_.pow = SingleCalculation::Lerp(MAX_VIGNETTE_POW_, 0.0f, vignetteChangeTime_);
	}
	//ピンチ演出
	else if (player_->GetHP() == DANGEROUS_HP) {
		warningTime_ += DELTA_TIME_;
		vignette_.pow = SingleCalculation::Lerp(MAX_VIGNETTE_POW_, 0.0f, warningTime_);

		//最大時間
		const float_t MAX_WARNING_TIME = 1.0f;
		//最小時間
		const float_t MIN_WARNING_TIME = 1.0f;

		if (warningTime_ > MAX_WARNING_TIME) {
			warningTime_ = MIN_WARNING_TIME;
		}
	}
	//通常時の場合
	else {
		vignette_.pow = 0.0f;
		vignetteChangeTime_ = 0.0f;
	}
	//ビネットの更新
	vignette_.Update();
}

void GameScene::DisplayImGui() {

	ImGui::Begin("ゲームシーン");

	if (ImGui::TreeNode("ビネット")==true) {
		ImGui::InputFloat("POW", &vignette_.pow);
		ImGui::InputFloat("変化の時間", &vignetteChangeTime_);
		ImGui::TreePop();
	}

	ImGui::End();

}

void GameScene::Update(Elysia::GameManager* gameManager) {

	//プレイヤーの更新
	player_->Update();
	//敵管理クラスの更新
	enemyManager_->Update();
	//敵を消す
	enemyManager_->DeleteEnemy();
	//鍵管理クラスの更新
	keyManager_->Update();

	//カメラの更新
	//レールカメラから2つの行列を取得
	camera_.viewMatrix = player_->GetEyeCamera()->GetCamera().viewMatrix;
	//転送
	camera_.Transfer();
	//レベルエディタで使うリスナーの設定
	Listener listener = {
		.position = player_->GetWorldPosition(),
		.move = player_->GetDirection(),
	};
	levelDataManager_->SetListener(levelHandle_, listener);
	//レベルエディタの更新
	levelDataManager_->Update(levelHandle_);

	//ビネットの処理
	VigntteProcess();

	//細かいシーンの更新
	detailGameScene_->Update(this);

	//処理が終わった時
	if (isEnd_ == true) {
		//勝ったとき
		if (isWin_ == true) {
			gameManager->ChangeScene("Win");
			return;
		}
		//負けた時
		if (isLose_ == true) {
			gameManager->ChangeScene("Lose");
			return;
		}
	}

	
#ifdef _DEBUG 

	//再読み込み
	if (input_->IsTriggerKey(DIK_R) == true) {
		levelDataManager_->Reload(levelHandle_);
	}
	//ImGuiの表示
	DisplayImGui();

#endif // _DEBUG
}

void GameScene::PreDrawPostEffect() {
	//ビネット描画処理前
	vignettePostEffect_->PreDraw();
}

void GameScene::DrawObject3D() {

	//懐中電灯を取得
	SpotLight spotLight = player_->GetFlashLight()->GetSpotLight();

	//レベルエディタ  
	levelDataManager_->Draw(levelHandle_, camera_, spotLight);
	//敵
	enemyManager_->DrawObject3D(camera_, spotLight);
	//プレイヤー
	player_->DrawObject3D(camera_, spotLight);
	//鍵
	keyManager_->DrawObject3D(camera_, spotLight);
}

void GameScene::DrawPostEffect() {
	//ビネット描画
	vignettePostEffect_->Draw(vignette_);
}

void GameScene::DrawSprite() {
	//細かいシーンのスプライト描画
	detailGameScene_->DrawSprite();

}
