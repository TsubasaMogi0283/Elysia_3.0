#include "ExplanationGameScene.h"

#include <imgui.h>

#include "Input.h"
#include "TextureManager.h"
#include "LevelDataManager.h"
#include "GameScene/GameScene.h"


ExplanationGameScene::ExplanationGameScene(){
	//インスタンスの取得
	//テクスチャ管理クラス
	textureManager_ = Elysia::TextureManager::GetInstance();
	//入力クラス
	input_ = Elysia::Input::GetInstance();
	//レベルエディタ管理クラス
	levelDataManager_ = Elysia::LevelDataManager::GetInstance();
}


void ExplanationGameScene::Initialize(){
	//説明画像の読み込み
	uint32_t explanationTextureHandle[EXPLANATION_QUANTITY_] = {};
	explanationTextureHandle[0] = textureManager_->Load("Resources/Sprite/Explanation/Explanation1.png");
	explanationTextureHandle[1] = textureManager_->Load("Resources/Sprite/Explanation/Explanation2.png");

	//生成
	for (uint32_t i = 0u; i < explanation_.size(); ++i) {
		explanation_[i].reset(Elysia::Sprite::Create(explanationTextureHandle[i], INITIAL_SPRITE_POSITION_));
	}

	//スペースで次への画像読み込み
	uint32_t spaceToNextTextureHandle[EXPLANATION_QUANTITY_] = {};
	spaceToNextTextureHandle[0] = textureManager_->Load("Resources/Sprite/Explanation/ExplanationNext1.png");
	spaceToNextTextureHandle[1] = textureManager_->Load("Resources/Sprite/Explanation/ExplanationNext2.png");
	//生成
	for (uint32_t i = 0u; i < spaceToNext_.size(); ++i) {
		spaceToNext_[i].reset(Elysia::Sprite::Create(spaceToNextTextureHandle[i], INITIAL_SPRITE_POSITION_));
	}

	//最初は0番目
	howToPlayTextureNumber_ = 0u;

	
	
}

void ExplanationGameScene::Update(GameScene* gameScene){
	//説明
	//スペースまたはBボタンを押したとき
	if (input_->IsTriggerKey(DIK_SPACE) == true || input_->IsTriggerButton(XINPUT_GAMEPAD_B) == true) {
		++howToPlayTextureNumber_;
	}

	//読み終わったらゲームプレイへ
	if (howToPlayTextureNumber_ > EXPLANATION_QUANTITY_) {
		gameScene;
		return;
	}



#ifdef _DEBUG
	//ImGui表示用
	DisplayImGui();
#endif // _DEBUG
}


void ExplanationGameScene::DrawSprite(){
	//説明
	for (size_t i = 0u; i < explanation_.size(); ++i) {
		if (howToPlayTextureNumber_ == i + 1u) {
			explanation_[i]->Draw();
			spaceToNext_[i]->Draw();
		}
	}

}

void ExplanationGameScene::DisplayImGui(){
	//int32_t型へ変換
	int32_t newHowToPlayTextureNumber = static_cast<int32_t>(howToPlayTextureNumber_);
	
	ImGui::Begin("説明(ゲーム)");
	ImGui::InputInt("テクスチャ番号", &newHowToPlayTextureNumber);


	ImGui::End();

}
