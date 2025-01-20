#include "GameManager.h"
#include "GameScene/GameScene.h"
#include "WinScene/WinScene.h"
#include "LoseScene/LoseScene.h"
#include "TitleScene/TitleScene.h"
#include "LevelEditorSample/LevelEditorSample.h"

#include "AbstractSceneFactory.h"


void GameManager::Initialize() {
	//シーンごとに動作確認したいときはここを変えてね
	currentGamaScene_ = std::make_unique<TitleScene>();
	
#ifdef _DEBUG

	currentGamaScene_ = std::make_unique<TitleScene>();
#endif // _DEBUG

	//初期化
	currentGamaScene_->Initialize();


}

void GameManager::ChangeScene(std::unique_ptr<IGameScene> newGameScene) {
	//前回と違った場合だけ通す
	if (currentGamaScene_ != newGameScene) {
		currentGamaScene_ = std::move(newGameScene);
		//引数が次に遷移するシーン
		currentGamaScene_->Initialize();
	}
	
}



void GameManager::ChangeScene(const std::string& sceneName){
	//現在入っているシーン名を更新
	currentSceneName_ = sceneName;

	//前回と違った場合だけ通す
	if (currentSceneName_ != sceneName) {
		currentGamaScene_ = abstractSceneFactory_->CreateScene(currentSceneName_);

		//空ではない時初期化処理に入る
		if (currentGamaScene_ != nullptr) {
			//引数が次に遷移するシーン
			currentGamaScene_->Initialize();
		}
	}
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



