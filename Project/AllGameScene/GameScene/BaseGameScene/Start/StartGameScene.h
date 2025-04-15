#pragma once


/**
 * @file StartGameScene.h
 * @brief ゲームの始めのクラス
 * @author 茂木翼
 */

#include "Sprite.h"

#include "GameScene/BaseGameScene/BaseGameScene.h"



/// <summary>
/// ゲームの始め
/// </summary>
class StartGameScene : public BaseGameScene {
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	StartGameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize()override;

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="gameScene">ゲームシーン(メイン)</param>
	void Update(GameScene* gameScene)override;

	/// <summary>
	/// スプライト
	/// </summary>
	void DrawSprite()override;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~StartGameScene()override=default;


private:
	/// <summary>
	/// ImGUi表示用
	/// </summary>
	void DisplayImGui()override;

private:

};