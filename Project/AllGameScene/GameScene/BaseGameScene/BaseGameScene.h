#pragma once
/**
 * @file BaseGameScene.h
 * @brief 細かいゲームシーンの基底クラス
 * @author 茂木翼
 */

#include <cstdint>
#include <memory>

#include "Sprite.h"

/// <summary>
/// ElysiaEngine
/// </summary>
namespace Elysia {
	/// <summary>
	/// 入力
	/// </summary>
	class Input;

	/// <summary>
	/// レベル管理クラス
	/// </summary>
	class LevelDataManager;

	/// <summary>
	/// ゲーム管理クラス
	/// </summary>
	class GameManager;

	/// <summary>
	/// テクスチャ管理クラス
	/// </summary>
	class TextureManager;


}

/// <summary>
/// ゲームシーン(メイン)
/// </summary>
class GameScene;

/// <summary>
/// 細かいゲームシーンの基底クラス
/// </summary>
class BaseGameScene {
public:

	/// <summary>
	/// 個別の初期化
	/// </summary>
	virtual void Initialize() = 0;

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="gameScene">ゲームシーン(メイン)</param>
	virtual void Update(GameScene* gameScene) = 0;

	/// <summary>
	/// スプライト
	/// </summary>
	virtual void DrawSprite() = 0;

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~BaseGameScene() = default;


protected:
	/// <summary>
	/// ImGui表示用
	/// </summary>
	virtual void DisplayImGui() = 0;


public:
	/// <summary>
	/// レベルデータハンドルの設定
	/// </summary>
	/// <param name="levelDataHandle">レベルデータハンドル</param>
	inline void SetLevelDataHandle(const uint32_t& levelDataHandle) {
		this->levelDataHandle_ = levelDataHandle;
	}



protected:
	//テクスチャ管理クラス
	Elysia::TextureManager* textureManager_ = nullptr;
	//入力クラス
	Elysia::Input* input_ = nullptr;
	//レベルエディタ
	Elysia::LevelDataManager* levelDataManager_ = nullptr;
	//ハンドル
	uint32_t levelDataHandle_ = 0u;


protected:
	//完全に透明になる値
	const float_t PERFECT_TRANSPARENT_ = 0.0f;
	//フェードの初期座標
	const Vector2 INITIAL_SPRITE_POSITION_ = { .x = 0.0f,.y = 0.0f };


};
