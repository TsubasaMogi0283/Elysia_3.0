#pragma once
/**
 * @file ContinueGameLoseScene.h
 * @brief ゲームを続けるクラス
 * @author 茂木翼
 */
#include "LoseScene/BaseLoseScene/BaseLoseScene.h"

/// <summary>
/// ゲームを続けるシーン
/// </summary>
class ContinueGameLoseScene : public BaseLoseScene {
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	ContinueGameLoseScene();

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
	~ContinueGameLoseScene()override= default;

private:
	//矢印の回転
	const float_t ROTATE_VALUE_ = 0.1f;
	//遷移する時間
	const float_t CHANGE_NEXTSCENE_TIME_ = 2.0f;
	//終わる時間
	const float_t FINISH_WAIT_TIME_ = 1.0f;
	//半径
	const float NARROW_VALUE = 0.002f;
private:
	//時間
	float_t waitForCameraMoveTime_ = 0u;
	//矢印の回転θ
	float_t arrowMoveTheta_ = 0.0f;
	//加速
	Vector3 cameraAccelation_ = { .x = 0.0f,.y = 0.001f,.z = -0.01f };
	float endLightUpT_ = 0.0f;
};

