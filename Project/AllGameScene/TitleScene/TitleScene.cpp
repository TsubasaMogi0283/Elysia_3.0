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

#include "BaseBackTexture/Sunset/SunsetBackTexture.h"
#include "BaseBackTexture/Night/NightBackTexture.h"

#include "BaseTitleScene/Start/StartTitleScene.h"

TitleScene::TitleScene(){
	//インスタンスの取得
	//テクスチャ管理クラス
	textureManager_ = Elysia::TextureManager::GetInstance();
	//入力クラス
	input_ = Elysia::Input::GetInstance();
	//レベルエディタ管理クラス
	levelDataManager_ = Elysia::LevelDataManager::GetInstance();
}

void TitleScene::Initialize(){

	//テクスチャハンドルの取得
	uint32_t textTextureHandle = textureManager_->Load("Resources/Sprite/Title/StartText.png");
	//タイトルテクスチャ
	uint32_t logoTextureHandle = textureManager_->Load("Resources/Sprite/Title/TitleNormal.png");
	changedLogoTextureHandle_ = textureManager_->Load("Resources/Sprite/Title/TitleChanged.png");
	logoTextureHandle_ = logoTextureHandle;
	//黒フェード
	uint32_t blackTexureHandle= textureManager_->Load("Resources/Sprite/Back/Black.png");

	//初期座標
	const Vector2 INITIAL_POSITION = {.x=0.0f,.y=0.0f};
	
	//テキスト
	text_.reset(Elysia::Sprite::Create(textTextureHandle, INITIAL_POSITION));
	//ロゴ
	logo.reset(Elysia::Sprite::Create(logoTextureHandle_, INITIAL_POSITION));
	//黒フェード
	blackFade_.reset(Elysia::Sprite::Create(blackTexureHandle, INITIAL_POSITION));
	//初期の透明度設定
	const float_t INITIAL_TRANSPARENCY = 0.0f;
	blackFade_->SetTransparency(INITIAL_TRANSPARENCY);

	//レベルデータの読み込み
	levelHandle_ = levelDataManager_->Load("TitleStage/TitleStage.json");


	isStart_ = false;
	isFlash_ = true;
	isFastFlash_ = false;

	
	//カメラの初期化
	camera_.Initialize();
	//座標
	camera_.translate = {.x = 0.0f,.y = 0.0f,.z = -30.8f };

	//レールカメラ
	titleRailCamera_ = std::make_unique<TitleRailCamera>();
	//初期化
	titleRailCamera_->Initialize("Resources/CSV/TitleRailCameraPoint.csv");
	

	//背景
	//ポストエフェクト
	//まずは夕方
	baseTitleBackTexture_ = std::make_unique<SunsetBackTexture>();
	baseTitleBackTexture_->Initialize();

	//ランダムエフェクトの生成
	randomEffect_ = std::make_unique<Elysia::RandomEffect>();
	//初期化
	randomEffect_->Initialize();
	
	//細かいシーンの初期化
	datailedTitleScene_ = std::make_unique<StartTitleScene>();
	datailedTitleScene_->CommonInitialize();
	datailedTitleScene_->Initialize();

	//シーンが変わった時に消えてしまうのでメインで宣言して使う

	//平行光源
	directionalLight_.Initialize();
	directionalLight_.color = { .x = 1.0f,.y = 0.22f,.z = 0.0f,.w = 1.0f };
	directionalLight_.direction = { .x = 0.91f,.y = -1.0f,.z = 0.0f };


}

void TitleScene::Update(Elysia::GameManager* gameManager){
	//細かいシーンの更新
	
	datailedTitleScene_->Update(this);
	datailedTitleScene_->CommoonUpdate();

#pragma region スタート演出

	//シーン遷移演出
	if (isStart_ == true) {

		//スタートのテキストを非表示にさせる
		logo->SetInvisible(false);
		text_->SetInvisible(true);

		//時間の加算
		const float_t DELTA_TIME = 1.0f/60.0f;
		randomEffectTime_ += DELTA_TIME;

		//開始時間
		std::array<float_t, DISPLAY_LENGTH_QUANTITY_> RANDOM_EFFECT_DISPLAY_START_TIME = { 0.0f,2.5f };
		//表示の長さ
		std::array<float_t, DISPLAY_LENGTH_QUANTITY_> RANDOM_EFFECT_DISPLAY_LENGTH = { 1.0f,3.0f };



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
				//光の強さと色を変え夜っぽくする
				directionalLight_.intensity = 0.05f;
				directionalLight_.color = { .x = 1.0f,.y = 1.0f,.z = 1.0f,.w = 1.0f };

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
			const float_t FADE_INCREASE_VALUE = 0.01f;
			blackFadeTransparency_ += FADE_INCREASE_VALUE;
			
		}

		//時間も兼ねている
		if (blackFadeTransparency_ > 2.0f) {
			gameManager->ChangeScene("Game");
			return;
		}
		
	}
#pragma endregion



	directionalLight_.Update();

#ifdef _DEBUG
	//ImGui用
	DisplayImGui();
#endif
}

void TitleScene::DrawObject3D(){
	
	//細かいシーン
	datailedTitleScene_->CommonDrawObject3D();
	datailedTitleScene_->DrawObject3D();

}

void TitleScene::PreDrawPostEffect(){
	//細かいシーン
	datailedTitleScene_->CommonPreDrawPostEffect();
	datailedTitleScene_->PreDrawPostEffect();

}

void TitleScene::DrawPostEffect(){
	//細かいシーン
	datailedTitleScene_->CommonDrawPostEffect();
	datailedTitleScene_->DrawPostEffect();
}

void TitleScene::DrawSprite(){
	//細かいシーン
	datailedTitleScene_->CommonDrawSprite();
	datailedTitleScene_->DrawSprite();
}

void TitleScene::DisplayImGui(){

	ImGui::Begin("タイトルシーン");
	if (ImGui::TreeNode("平行光源") == true) {
		ImGui::SliderFloat4("色", &directionalLight_.color.x, 0.0f, 1.0f);
		ImGui::SliderFloat3("方向", &directionalLight_.direction.x, -1.0f, 1.0f);
		ImGui::SliderFloat("強さ", &directionalLight_.intensity, 0.0f, 1.0f);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("エフェクト") == true) {
		ImGui::InputFloat("Count", &randomEffectTime_);
		ImGui::Checkbox("IsDisplay", &isDisplayRandomEffect_);
		ImGui::TreePop();
	}
	
	
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

void TitleScene::ChangeDetailedScene(std::unique_ptr<BaseTitleScene> detailedScene){
	//違った時だけ遷移する
	if (datailedTitleScene_ != detailedScene) {
		datailedTitleScene_ = std::move(detailedScene);
		//個別の初期化
		datailedTitleScene_->Initialize();
	}
	
}
