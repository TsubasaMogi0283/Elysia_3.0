#include "TitleScene.h"
#include <imgui.h>
#include <numbers>
#include <array>

#include "Input.h"
#include "GameManager.h"
#include "TextureManager.h"
#include "LevelDataManager.h"
#include "VectorCalculation.h"
#include "SingleCalculation.h"
#include "Calculation/QuaternionCalculation.h"


#include "BaseBackTexture/Sunset/SunsetBackTexture.h"
#include "BaseBackTexture/Night/NightBackTexture.h"


#include "BaseTitleScene/Waiting/WaitingTitleScene.h"

TitleScene::TitleScene() {
	//インスタンスの取得
	//テクスチャ管理クラス
	textureManager_ = Elysia::TextureManager::GetInstance();
	//入力クラス
	input_ = Elysia::Input::GetInstance();
	//レベルエディタ管理クラス
	levelDataManager_ = Elysia::LevelDataManager::GetInstance();
}

void TitleScene::Initialize() {

	//テクスチャハンドルの取得
	uint32_t textTextureHandle = textureManager_->Load("Resources/Sprite/Title/StartText.png");
	//タイトルテクスチャ
	uint32_t logoTextureHandle = textureManager_->Load("Resources/Sprite/Title/TitleNormal.png");
	changedLogoTextureHandle_ = textureManager_->Load("Resources/Sprite/Title/TitleChanged.png");
	logoTextureHandle_ = logoTextureHandle;
	//黒フェード
	uint32_t blackTexureHandle = textureManager_->Load("Resources/Sprite/Back/Black.png");

	//初期座標
	const Vector2 INITIAL_POSITION = { .x = 0.0f,.y = 0.0f };

	//テキスト
	text_.reset(Elysia::Sprite::Create(textTextureHandle, INITIAL_POSITION));
	//ロゴ
	logo.reset(Elysia::Sprite::Create(logoTextureHandle_, INITIAL_POSITION));
	//黒フェード
	blackFade_.reset(Elysia::Sprite::Create(blackTexureHandle, INITIAL_POSITION));
	//初期の透明度設定
	const float INITIAL_TRANSPARENCY = 0.0f;
	blackFade_->SetTransparency(INITIAL_TRANSPARENCY);

	//レベルデータの読み込み
	levelHandle_ = levelDataManager_->Load("TitleStage/TitleStage.json");


	isStart_ = false;
	isFlash_ = true;
	isFastFlash_ = false;

	//平行光源
	directionalLight_.Initialize();
	directionalLight_.color = { .x = 1.0f,.y = 0.22f,.z = 0.0f,.w = 1.0f };
	directionalLight_.direction = { .x = 0.91f,.y = -1.0f,.z = 0.0f };

	//カメラの初期化
	camera_.Initialize();
	//座標
	camera_.translate = { .x = 0.0f,.y = 0.0f,.z = -30.8f };

	//レールカメラ
	titleRailCamera_ = std::make_unique<TitleRailCamera>();
	//初期化
	titleRailCamera_->Initialize("Resources/CSV/TitleRailCameraPoint.csv");


	//背景
	//ポストエフェクト
	baseTitleBackTexture_ = std::make_unique<SunsetBackTexture>();
	baseTitleBackTexture_->Initialize();

	//ランダムエフェクトの生成
	randomEffect_ = std::make_unique<Elysia::RandomEffect>();
	//初期化
	randomEffect_->Initialize();


	//細かいシーン
	detailTitleScene_ = std::make_unique<WaitingTitleScene>();
	//レベルデータハンドルの設定
	detailTitleScene_->SetLevelDataHandle(levelHandle_);
	//初期化
	detailTitleScene_->Initialize();


}

void TitleScene::Update(Elysia::GameManager* gameManager) {
	//細かいシーンの更新
	detailTitleScene_->Update(this);
	//処理を終えたらゲームシーンへ
	if(isEnd_ == true) {
		gameManager->ChangeScene("Game");
		return;
	}
	

	

	//更新
	levelDataManager_->Update(levelHandle_);

	//平行光源
	directionalLight_.color = detailTitleScene_->SetDirectionalLight().color;
	directionalLight_.direction = detailTitleScene_->SetDirectionalLight().direction;
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

void TitleScene::DrawObject3D() {
	//ステージオブジェクト
	levelDataManager_->Draw(levelHandle_, camera_, directionalLight_);

}

void TitleScene::PreDrawPostEffect() {

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

void TitleScene::DrawPostEffect() {
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

void TitleScene::DrawSprite() {
	//細かいシーンの描画
	detailTitleScene_->DrawSprite();

}

void TitleScene::DisplayImGui() {
	ImGui::Begin("TitleFade&Effect");
	ImGui::InputFloat("Count", &randomEffectTime_);
	ImGui::Checkbox("IsDisplay", &isDisplayRandomEffect_);
	ImGui::End();

	ImGui::Begin("平行光源");
	ImGui::SliderFloat4("色", &directionalLight_.color.x, 0.0f, 1.0f);
	ImGui::SliderFloat3("方向", &directionalLight_.direction.x, -1.0f, 1.0f);
	ImGui::End();

}


void TitleScene::ChangeBackTexture(std::unique_ptr<BaseTitleBackTexture> backTexture) {
	//違った時だけ遷移する
	if (baseTitleBackTexture_ != backTexture) {
		baseTitleBackTexture_ = std::move(backTexture);
		//次に遷移する
		baseTitleBackTexture_->Initialize();
	}
}

void TitleScene::ChangeDetailScene(std::unique_ptr<BaseTitleScene> detailTitleScene){
	//違った時だけ遷移する
	if (detailTitleScene_ != detailTitleScene) {
		detailTitleScene_ = std::move(detailTitleScene);
		//レベルデータハンドルの設定
		detailTitleScene_->SetLevelDataHandle(levelHandle_);
		//次に遷移する
		detailTitleScene_->Initialize();
	}
}

