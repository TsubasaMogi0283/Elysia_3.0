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
	/// 初期化(1度きり)
	/// </summary>
	virtual void Initialize();

	/// <summary>
	/// 個別の初期化
	/// </summary>
	virtual void IndivisualInitialize() = 0;

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
	virtual void PreDrawPostEffect()=0;

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


protected:
	//↓↓↓タイトルシーンで使う共通の部分はここで宣言してそれぞれで使うようにする↓↓↓

	//カメラ
	Camera camera_ = {};
	std::unique_ptr<TitleRailCamera> titleRailCamera_ = nullptr;

	//座標
	Vector3 cameraPosition_ = {};

	//平行光源
	DirectionalLight directionalLight_ = {};


	//黒フェード
	std::unique_ptr<Elysia::Sprite>blackFade_ = nullptr;
	//透明度
	float_t blackFadeTransparency_ = 0.0f;


	//テキスト
	std::unique_ptr<Elysia::Sprite> text_ = nullptr;

	//ロゴ
	std::unique_ptr<Elysia::Sprite> logo = nullptr;
	uint32_t logoTextureHandle_ = 0u;
	//変化後のテクスチャ
	uint32_t changedLogoTextureHandle_ = 0u;


};
