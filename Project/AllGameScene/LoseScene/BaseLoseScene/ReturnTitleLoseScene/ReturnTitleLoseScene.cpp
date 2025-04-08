#include "ReturnTitleLoseScene.h"

#include <numbers>

#include "Input.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "LevelDataManager.h"
#include "GlobalVariables.h"

#include "Easing.h"
#include "VectorCalculation.h"
#include "LoseScene/LoseScene.h"

ReturnTitleLoseScene::ReturnTitleLoseScene() {
	//インスタンスの取得
	//入力クラス
	input_ = Elysia::Input::GetInstance();
	//テクスチャ管理クラス
	textureManager_ = Elysia::TextureManager::GetInstance();
	//レベルデータ管理クラス
	levelDataManager_ = Elysia::LevelDataManager::GetInstance();
	//モデル管理クラス
	modelManager_ = Elysia::ModelManager::GetInstance();
	//グローバル変数クラス
	globalVariables_ = Elysia::GlobalVariables::GetInstance();
}


void ReturnTitleLoseScene::Initialize(){

	//矢印の初期座標を取得
	arrowInitialPosition_ = levelDataManager_->GetInitialTranslate(levelDataHandle_, SELECT_ARROW);
	//「タイトルへ」の座標を取得
	toTitleInitialPosition_ = levelDataManager_->GetInitialTranslate(levelDataHandle_, TO_TITLE);


	//点光源の半径を最大に設定
	pointLight_.radius = MAX_LIGHT_RADIUS_;
}

void ReturnTitleLoseScene::Update(LoseScene* loseScene){

	
	//新しい座標を設定
	levelDataManager_->SetTranslate(levelDataHandle_, SELECT_ARROW, { .x = toTitleInitialPosition_.x,.y = arrowInitialPosition_.y ,.z = arrowInitialPosition_.z });
	//カメラの動きを待つ時間
	changeWaitingTime_ += DELTA_TIME_;

	//指定した時間を超えたらタイトルへ遷移
	if (changeWaitingTime_ > FINISH_WAIT_TIME_) {
		dissolveThresholdT_ += THRESHOLD_INCREASE_AMOUNT_;
		//ディゾルブの値を増やす
		dissolve_.threshold = Easing::EaseInSine(dissolveThresholdT_);
		//暗くなったらシーンチェンジ
		fadeTransparency_ += TRANSPARENCY_INTERVAL_;
	}

	//完全い不透明になったら終了
	if (fadeTransparency_ > MAX_T_VALUE_) {
		loseScene->SetIsEnd();
	}
}
