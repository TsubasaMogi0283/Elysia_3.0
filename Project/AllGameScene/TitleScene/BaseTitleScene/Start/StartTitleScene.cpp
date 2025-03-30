#include "StartTitleScene.h"

#include "Input.h"
#include "TextureManager.h"
#include "LevelDataManager.h"
#include "WindowsSetup.h"

#include "TitleScene/TitleScene.h"
#include "TitleScene/BaseTitleScene/Waiting/WaitingTitleScene.h"

StartTitleScene::StartTitleScene(){
	//インスタンスの取得
	//テクスチャ管理クラス
	textureManager_ = Elysia::TextureManager::GetInstance();
	//入力クラス
	input_ = Elysia::Input::GetInstance();
	//レベルエディタ管理クラス
	levelDataManager_ = Elysia::LevelDataManager::GetInstance();
	//ウィンドウクラス
	windowsSetup_ = Elysia::WindowsSetup::GetInstance();
}

void StartTitleScene::IndivisualInitialize(){

	//黒い円
	uint32_t blackCircleTextureHandle = textureManager_->Load("Resources/Sprite/SceneChange/BlackCircle.png");
	//画面の中心の座標を取りたいのでウィンドウクラスから座標を持ってくる
	const Vector2 CIRCLE_POSITION = { .x = static_cast<float_t>(windowsSetup_->GetClientWidth()) / 2.0f,.y = static_cast<float_t>(windowsSetup_->GetClientHeight()) / 2.0f };
	//アンカーポイント
	const Vector2 CIRCLE_ANCHOR_POINT = { .x = 0.5f,.y = 0.5f };
	
	//生成
	circleSprite_.reset(Elysia::Sprite::Create(blackCircleTextureHandle, CIRCLE_POSITION));
	circleSprite_->SetAnchorPoint(CIRCLE_ANCHOR_POINT);
	//サイズの設定
	circleScaleSize_ = 50.0f;
	circleSprite_->SetScale({.x= circleScaleSize_,.y= circleScaleSize_ });

}

void StartTitleScene::Update(TitleScene* titleScene){
	//円が小さくなっていく
	circleScaleSize_ -= SIZE_DOWN_VALUE_;
	circleSprite_->SetScale({ .x = circleScaleSize_,.y = circleScaleSize_ });


	if (circleScaleSize_ < 0.0f) {
		circleScaleSize_ = 0.0f;
		//待ち状態へ
		//待ちと言ってもボタンを押してスタートのテキストが出るというもの
		titleScene->ChangeDetailedScene(std::make_unique<WaitingTitleScene>());
		return;
	}
	

	
}

void StartTitleScene::DrawObject3D(){

}

void StartTitleScene::PreDrawPostEffect(){

}

void StartTitleScene::DrawPostEffect(){

}

void StartTitleScene::DrawSprite(){
	//背景
	logo->Draw(logoTextureHandle_);
	//テキスト
	text_->Draw();

	//黒い円の描画
	circleSprite_->Draw();
}
