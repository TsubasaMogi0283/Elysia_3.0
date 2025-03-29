#pragma once


/**
 * @file StartTitleScene.h
 * @brief タイトルの始めのクラス
 * @author 茂木翼
 */


#include "TitleScene/BaseTitleScene/BaseTitleScene.h"

/// <summary>
/// ELysiaEngine
/// </summary>
namespace Elysia {

	/// <summary>
	/// ウィンドウクラス
	/// </summary>
	class WindowsSetup;
}

/// <summary>
/// タイトルの始め
/// </summary>
class StartTitleScene : public BaseTitleScene {
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	StartTitleScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void IndivisualInitialize()override;

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="titleScene">タイトルシーン(メイン)</param>
	void Update(TitleScene* titleScene)override;

	/// <summary>
	/// 3Dオブジェクト
	/// </summary>
	void DrawObject3D()override;

	/// <summary>
	/// ポストエフェクト描画処理前
	/// </summary>
	void PreDrawPostEffect()override;

	/// <summary>
	/// ポストエフェクト描画
	/// </summary>
	void DrawPostEffect()override;


	/// <summary>
	/// スプライト
	/// </summary>
	void DrawSprite()override;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~StartTitleScene()override=default;

private:
	//ウィンドウクラス
	Elysia::WindowsSetup* windowsSetup_ = nullptr;

private:
	//サイズの減少値
	const float_t SIZE_DOWN_VALUE_ = 1.0f;
	//最小サイズ
	const float_t CIRCLE_MIN_SIZE_ = 0.0f;

private:

	//円
	std::unique_ptr<Elysia::Sprite>circleSprite_ = nullptr;
	//透明度
	float_t circleScaleSize_ = 50.0f;
};