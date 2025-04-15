#pragma once


/**
 * @file StartTitleScene.h
 * @brief タイトルの始めのクラス
 * @author 茂木翼
 */

#include "Sprite.h"

#include "TitleScene/BaseTitleScene/BaseTitleScene.h"

/// <summary>
/// ElysiaEngine
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
	void Initialize()override;

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="titleScene">タイトルシーン(メイン)</param>
	void Update(TitleScene* titleScene)override;

	/// <summary>
	/// スプライト
	/// </summary>
	void DrawSprite()override;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~StartTitleScene()override=default;


private:
	/// <summary>
	/// ImGUi表示用
	/// </summary>
	void DisplayImGui()override;

private:
	//ウィンドウクラス
	Elysia::WindowsSetup* windowsSetup_ = nullptr;

private:
	//サイズの減少値
	const float_t SIZE_DOWN_VALUE_ = 0.05f;
	//最小サイズ
	const float_t CIRCLE_MIN_SIZE_ = 0.0f;

private:

	//円
	std::unique_ptr<Elysia::Sprite>circleSprite_ = nullptr;
	//透明度
	float_t circleScaleSize_ = 15.0f;
};