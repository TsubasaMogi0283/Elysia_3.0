#pragma once

#include "TitleScene/BaseTitleScene/BaseTitleScene.h"


/// <summary>
/// スタート待ちのタイトルシーン
/// </summary>
class WaitingTitleScene: public BaseTitleScene {
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

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
	void PreDrawPostEffectFirst();

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

};