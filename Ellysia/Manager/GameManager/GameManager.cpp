#include "GameManager.h"
#include <cassert>
#include "GameSceneFactory.h"


void GameManager::Initialize() {
	
	//シーンファクトリーの生成
	abstractSceneFactory_ = std::make_unique<GameSceneFactory>();
	
	//シーンごとに動作確認したいときはここを変えてね
	currentGamaScene_ = abstractSceneFactory_->CreateScene("Game");

#ifdef _DEBUG
	currentGamaScene_ = abstractSceneFactory_->CreateScene("Win");

#endif // _DEBUG

	//初期化
	currentGamaScene_->Initialize();


}



void GameManager::ChangeScene(const std::string& sceneName){
	//新しいシーンに遷移するためにPreの所に入っていたものを入れる
	preSceneName_ = currentSceneName_;
	//現在入っているシーン名を更新
	currentSceneName_ = sceneName;

	//シーンの値を取ってくる
	currentGamaScene_ = abstractSceneFactory_->CreateScene(currentSceneName_);
	//空ではない時初期化処理に入る
	assert(currentGamaScene_ != nullptr);
	//初期化
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



