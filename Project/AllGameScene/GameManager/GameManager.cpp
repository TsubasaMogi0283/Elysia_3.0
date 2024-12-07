#include "GameManager.h"
#include "GameScene/GameScene.h"
#include "WinScene/WinScene.h"
#include "LoseScene/LoseScene.h"
#include "TitleScene/TitleScene.h"
#include "LevelEditorSample/LevelEditorSample.h"

//シーンファクトリー作った方がよさそう

void GameManager::Initialize() {
	//シーンごとに動作確認したいときはここを変えてね
	currentGamaScene_ = new TitleScene();
	
#ifdef _DEBUG
	//currentGamaScene_ = new LevelEditorSample();
	currentGamaScene_ = new LoseScene();
#endif // _DEBUG

	//初期化
	currentGamaScene_->Initialize();

	sceneQuantity_ = 0;

}

void GameManager::ChangeScene(IGameScene* newGameScene) {
	//一度消してから次のシーンにいく
	delete currentGamaScene_;
	currentGamaScene_ = newGameScene;
	//引数が今入っているシーン
	currentGamaScene_->Initialize();
}



void GameManager::Update() {

#ifdef _DEBUG
	
	const char* sceneName[] = { "Title", "Game", "Win" };
	//シーンファクトリー組み合わせたらデバッグがやりやすそう
	//選択するだけでシーン遷移出来るから
	ImGui::Begin("Scene");
	if (ImGui::BeginCombo("SceneSelect", sceneName[sceneQuantity_])) {
		for (uint32_t i = 0; i < IM_ARRAYSIZE(sceneName); i++) {
			bool isSelected = (sceneQuantity_ == i);
			if (ImGui::Selectable(sceneName[i], isSelected))
			{
				sceneQuantity_ = i; // 選択されたアイテムのインデックスを更新
			}

			// 現在選択されているアイテムにフォーカスを設定
			if (isSelected==true) {
				ImGui::SetItemDefaultFocus();
			}
			
		}

		ImGui::EndCombo();
	}
	ImGui::End();
#endif // _DEBUG


	currentGamaScene_->Update(this);
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
