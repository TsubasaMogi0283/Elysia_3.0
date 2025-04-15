#include "StartGameScene.h"

#include "Input.h"
#include "TextureManager.h"
#include "LevelDataManager.h"
#include "WindowsSetup.h"

#include "GameScene/GameScene.h"


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

}

void StartGameScene::Update(GameScene* gameScene){




#ifdef _DEBUG
	//ImGui表示用
	DisplayImGui();
#endif // _DEBUG
}


void StartGameScene::DrawSprite(){

}

void StartGameScene::DisplayImGui(){
	ImGui::Begin("開始(タイトル)");

	ImGui::End();


	
}
