#include "GameManager.h"
#include <cassert>
#include <imgui.h>

#include "GameSceneFactory.h"
#include <vector>


void Ellysia::GameManager::Initialize() {
	
	//シーンファクトリーの生成
	abstractSceneFactory_ = std::make_unique<GameSceneFactory>();
	
	//シーンごとに動作確認したいときはここを変えてね
	currentGamaScene_ = abstractSceneFactory_->CreateScene("Game");

#ifdef _DEBUG
	//デバッグ時はこっちに入れてね
	currentGamaScene_ = abstractSceneFactory_->CreateScene("Lose");
#endif // _DEBUG

	//初期化
	currentGamaScene_->Initialize();


}



void Ellysia::GameManager::ChangeScene(const std::string& sceneName){
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

void Ellysia::GameManager::Update() {
	//更新
	currentGamaScene_->Update(this);

#ifdef _DEBUG
	ImGui::Begin("ゲームシーンの管理");
	const char* SCENE_NAME[] = {"Title","Game","Win","Lose"};
	if (ImGui::BeginCombo("シーン", SCENE_NAME[currentSceneNumber_])) {
		for (uint32_t i = 0u; i < IM_ARRAYSIZE(SCENE_NAME); ++i) {
			bool isSelected = (currentSceneNumber_ == i);

			if (ImGui::Selectable(SCENE_NAME[i], isSelected)){
				// 選択されたアイテムのインデックスを更新する
				currentSceneNumber_ = i; 
				ChangeScene(SCENE_NAME[i]);
			}

			// 現在選択されているアイテムにフォーカスを設定する
			if (isSelected == true) {
				ImGui::SetItemDefaultFocus();
			}

		}

		ImGui::EndCombo();
	}

	ImGui::End();
#endif // _DEBUG


}

void Ellysia::GameManager::DrawObject3D() {
	//3Dオブジェクトの描画
	currentGamaScene_->DrawObject3D();
}

void Ellysia::GameManager::DrawSprite(){
	//スプライトの描画
	currentGamaScene_->DrawSprite();
}

void Ellysia::GameManager::PreDrawPostEffectFirst(){
	//ポストエフェクト描画処理前
	currentGamaScene_->PreDrawPostEffectFirst();
}


void Ellysia::GameManager::DrawPostEffect(){
	//ポストエフェクト描画前
	currentGamaScene_->DrawPostEffect();
}



