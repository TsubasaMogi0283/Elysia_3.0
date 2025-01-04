#include "GameManager.h"
#include "GameScene/GameScene.h"
#include "WinScene/WinScene.h"
#include "LoseScene/LoseScene.h"
#include "TitleScene/TitleScene.h"
#include "LevelEditorSample/LevelEditorSample.h"



void GameManager::Initialize() {
	//シーンごとに動作確認したいときはここを変えてね
	currentGamaScene_ = new GameScene();
	
#ifdef _DEBUG
	//currentGamaScene_ = new LevelEditorSample();
	currentGamaScene_ = new GameScene();
	//currentGamaScene_ = new TitleScene();
#endif // _DEBUG

	//初期化
	currentGamaScene_->Initialize();


}

void GameManager::ChangeScene(IGameScene* newGameScene) {
	//一度消してから次のシーンにいく
	delete currentGamaScene_;
	currentGamaScene_ = newGameScene;
	//引数が今入っているシーン
	currentGamaScene_->Initialize();
}



void GameManager::Update() {
	//更新
	currentGamaScene_->Update(this);
}

void GameManager::DrawObject3D() {
	//3Dオブジェクトの描画
	currentGamaScene_->DrawObject3D();
}

void GameManager::DrawSprite(){
	//スプライトの描画
	currentGamaScene_->DrawSprite();
}

void GameManager::PreDrawPostEffectFirst(){
	//ポストエフェクト描画処理前
	currentGamaScene_->PreDrawPostEffectFirst();
}


void GameManager::DrawPostEffect(){
	//ポストエフェクト描画前
	currentGamaScene_->DrawPostEffect();
}



GameManager::~GameManager() {
	//消去
	delete currentGamaScene_;
}
