#pragma once


/**
 * @file StartTitleScene.h
 * @brief ノイズのクラス
 * @author 茂木翼
 */


#include "TitleScene/BaseTitleScene/BaseTitleScene.h"


/// <summary>
/// ノイズ
/// </summary>
class NoiseTitleScene : public BaseTitleScene {
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	NoiseTitleScene();

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
	~NoiseTitleScene()override;

};

