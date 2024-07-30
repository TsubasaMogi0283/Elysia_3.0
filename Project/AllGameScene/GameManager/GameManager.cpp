#include "GameManager.h"
#include "SampleScene/SampleScene.h"
#include "SampleScene2/SampleScene2.h"

#include "TitleScene/TitleScene.h"

void GameManager::Initialize() {
	//シーンごとに動作確認したいときはここを変えてね
	currentGamaScene_ = new TitleScene();

#ifdef _DEBUG
	currentGamaScene_ = new SampleScene();
#endif // _DEBUG


	currentGamaScene_->Initialize();

}

void GameManager::ChangeScene(IGameScene* newGameScene) {
	//一度消してから次のシーンにいく
	delete currentGamaScene_;
	currentGamaScene_ = newGameScene;
	//今は言っているシーンが引数
	currentGamaScene_->Initialize();
}



void GameManager::Update() {
	currentGamaScene_->Update(this);
}

void GameManager::DrawSpriteBack(){
	currentGamaScene_->DrawSpriteBack();
}

void GameManager::DrawObject3D() {
	currentGamaScene_->DrawObject3D();
}

void GameManager::DrawSprite(){
	currentGamaScene_->DrawSprite();
}

void GameManager::PreDrawPostEffectFirst(){
	currentGamaScene_->PreDrawPostEffectFirst();
}


void GameManager::DrawPostEffect(){
	currentGamaScene_->DrawPostEffect();
}



GameManager::~GameManager() {
	delete currentGamaScene_;

}
