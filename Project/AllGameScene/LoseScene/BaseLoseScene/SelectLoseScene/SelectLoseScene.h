#pragma once
/**
 * @file SelectLoseScene.h
 * @brief 選択シーンのクラス
 * @author 茂木翼
 */
#include "LoseScene/BaseLoseScene/BaseLoseScene.h"

/// <summary>
/// 選択シーン
/// </summary>
class SelectLoseScene : public BaseLoseScene {
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	SelectLoseScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize()override;

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="loseScene">失敗シーン(メイン)</param>
	void Update(LoseScene* loseScene) override;


	/// <summary>
	/// デストラクタ
	/// </summary>
	~SelectLoseScene()override= default;

private:
	//矢印の回転
	const float_t ROTATE_VALUE_ = 0.1f;
	//遷移する時間
	const float_t CHANGE_NEXTSCENE_TIME_ = 2.0f;
	//
	const float MOVE_AMOUNT = 0.1f;
	//高速回転
	const float FAST_ROTATE_VALUE = 0.5f;
private:
	//矢印の回転
	float_t arrowRotate_ = 0.0f;
	//選択時のスケール
	float_t selectedScale_ = 1.5f;

	//選択中
	//ゲームを続けるかどうか
	bool isContinue_ = false;

	//次へ
	bool isChangeNextScene_ = false;
	//決定時の動きに使うθ
	float_t decideArrowMoveTheta_ = 0.0f;


	//遷移まで待つ時間
	float_t waitForNextSceneTime_ = 0.0f;
};

