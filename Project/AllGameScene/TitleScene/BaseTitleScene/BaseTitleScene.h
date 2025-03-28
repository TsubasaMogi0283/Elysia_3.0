#pragma once
/**
 * @file BaseTitleScene.h
 * @brief 細かいタイトルシーンの基底クラス
 * @author 茂木翼
 */

#include <cstdint>

#include "Model.h"
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
/// タイトルシーン(メイン)
/// </summary>
class TitleScene;

/// <summary>
/// 細かいタイトルシーンの基底クラス
/// </summary>
class BaseTitleScene {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize()=0;

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="titleScene">タイトルシーン(メイン)</param>
	virtual void Update(TitleScene* titleScene)=0;

	/// <summary>
	/// 3Dオブジェクト
	/// </summary>
	virtual void DrawObject3D()=0;

	/// <summary>
	/// ポストエフェクト描画処理前
	/// </summary>
	virtual void PreDrawPostEffectFirst()=0;

	/// <summary>
	/// ポストエフェクト描画
	/// </summary>
	virtual void DrawPostEffect()=0;


	/// <summary>
	/// スプライト
	/// </summary>
	virtual void DrawSprite()=0;

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~BaseTitleScene() = default;

protected:
	//テクスチャ管理クラス
	Elysia::TextureManager* textureManager_ = nullptr;
	//入力クラス
	Elysia::Input* input_ = nullptr;
	//レベルエディタ
	Elysia::LevelDataManager* levelDataManager_ = nullptr;
	uint32_t levelHandle_ = 0u;


};
