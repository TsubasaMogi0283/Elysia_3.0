#pragma once

/**
 * @file TitleScene.h
 * @brief タイトルクラス
 * @author 茂木翼
 */


#include "TitleScene/BaseTitleScene/BaseTitleScene.h"


/// <summary>
/// スタート待ちのタイトルシーン
/// </summary>
class WaitingTitleScene: public BaseTitleScene {
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	WaitingTitleScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize()override;

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="titleScene">タイトルシーン(メイン)</param>
	void Update(TitleScene* titleScene);

	/// <summary>
	/// 3Dオブジェクト
	/// </summary>
	void DrawObject3D();

	/// <summary>
	/// ポストエフェクト描画処理前
	/// </summary>
	void PreDrawPostEffect();

	/// <summary>
	/// ポストエフェクト描画
	/// </summary>
	void DrawPostEffect();


	/// <summary>
	/// スプライト
	/// </summary>
	void DrawSprite();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~WaitingTitleScene()override;

private:
	
	//点滅の制限
	const uint32_t FLASH_TIME_LIMIT_ = 30u;
	//高速点滅の制限
	const uint32_t FAST_FLASH_TIME_LIMIT_ = 60u;
	//高速点滅の間隔
	const uint32_t FAST_FLASH_TIME_INTERVAL_ = 3u;
private:
	//点滅
	bool isFlash_ = false;
	//時間
	uint32_t flashTime_ = 0u;
	
	//高速点滅
	bool isFastFlash_ = false;
	//時間
	uint32_t fastFlashTime_ = 0u;


	//見せる番号
	uint32_t textDisplayCount_ = 0;
};