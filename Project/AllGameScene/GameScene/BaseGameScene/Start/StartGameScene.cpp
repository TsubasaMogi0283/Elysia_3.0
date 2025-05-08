#include "StartGameScene.h"

#include <imgui.h>

#include "Input.h"
#include "TextureManager.h"
#include "LevelDataManager.h"
#include "GameScene/GameScene.h"
#include "GameScene/BaseGameScene/Explanation/ExplanationGameScene.h"

StartGameScene::StartGameScene(){
	//インスタンスの取得
	//テクスチャ管理クラス
	textureManager_ = Elysia::TextureManager::GetInstance();
	//入力クラス
	input_ = Elysia::Input::GetInstance();
	//レベルエディタ管理クラス
	levelDataManager_ = Elysia::LevelDataManager::GetInstance();
}


void StartGameScene::Initialize(){

	//黒フェード
	//黒画像読み込み
	uint32_t blackTextureHandle = textureManager_->Load("Resources/Sprite/Back/Black.png");
	//生成
	blackFadeSprite_.reset(Elysia::Sprite::Create(blackTextureHandle, INITIAL_SPRITE_POSITION_));
	//初期の透明度を設定
	fadeTransparency_ = 1.0f;
	blackFadeSprite_->SetTransparency(fadeTransparency_);
}

void StartGameScene::Update(GameScene* gameScene){
	//透明度の設定
	//徐々に透明になっていく
	fadeTransparency_ -= FADE_VALUE_;
	blackFadeSprite_->SetTransparency(fadeTransparency_);

	//完全に透明になったらゲームが始まる
	if (fadeTransparency_ < PERFECT_TRANSPARENT_) {
		fadeTransparency_ = PERFECT_TRANSPARENT_;
		//説明シーンへ
		gameScene->ChangeDetailScene(std::make_unique<ExplanationGameScene>());
		return;
	}

#ifdef _DEBUG
	//ImGui表示用
	DisplayImGui();
#endif // _DEBUG
}


void StartGameScene::DrawSprite(){
	blackFadeSprite_->Draw();
}

void StartGameScene::DisplayImGui(){
	ImGui::Begin("開始(ゲーム)");
	ImGui::InputFloat("フェードの透明度", &fadeTransparency_);
	ImGui::End();

}
