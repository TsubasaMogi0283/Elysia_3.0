#include "BaseTitleScene.h"

#include "Input.h"
#include "TextureManager.h"
#include "LevelDataManager.h"
#include "TitleScene/BaseBackTexture/Sunset/SunsetBackTexture.h"

void BaseTitleScene::CommonInitialize(){
	//モデルとかスプライトの初期化はここで全て共通化させる
	//わざわざ個々のクラスで初期化していたら時間がかかるし変わった時に重くなる回数が多くなると思うので
	//インスタンスはインクルードの関係でここでやることにしたのでここではやっていない


	//インスタンスの取得
	//テクスチャ管理クラス
	textureManager_ = Elysia::TextureManager::GetInstance();
	//入力クラス
	input_ = Elysia::Input::GetInstance();
	//レベルエディタ管理クラス
	levelDataManager_ = Elysia::LevelDataManager::GetInstance();


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
	const float_t INITIAL_TRANSPARENCY = 0.0f;
	blackFade_->SetTransparency(INITIAL_TRANSPARENCY);

	//レベルデータの読み込み
	levelHandle_ = levelDataManager_->Load("TitleStage/TitleStage.json");


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
	//まずは夕方
	baseTitleBackTexture_ = std::make_unique<SunsetBackTexture>();
	baseTitleBackTexture_->Initialize();
}

void BaseTitleScene::CommoonUpdate(){
	//更新
	levelDataManager_->Update(levelHandle_);

	//平行光源
	directionalLight_.Update();

	//レールカメラの更新
	titleRailCamera_->Update();
	
	//カメラの更新
	camera_.Transfer();
}

void BaseTitleScene::CommonDrawObject3D(){
	//ステージオブジェクト
	levelDataManager_->Draw(levelHandle_, camera_, directionalLight_);
}

void BaseTitleScene::CommonPreDrawPostEffect(){
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


void BaseTitleScene::CommonDrawPostEffect(){

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

void BaseTitleScene::CommonDrawSprite(){

}
