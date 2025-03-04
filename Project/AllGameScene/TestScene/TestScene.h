#pragma once

#include <memory>

#include "IGameScene.h"
#include "BackTexture.h"
#include "Model.h"
#include "WorldTransform.h"
#include "Camera.h"
#include "Material.h"
#include "DirectionalLight.h"

/// <summary>
/// EllysiaEngine(前方宣言)
/// </summary>
namespace Ellysia {
	/// <summary>
	/// レベルエディタ
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

	/// <summary>
	/// モデル管理クラス
	/// </summary>
	class ModelManager;

	/// <summary>
	/// 入力クラス
	/// </summary>
	class Input;

	/// <summary>
	/// 調整項目クラス
	/// </summary>
	class GlobalVariables;
}


class TestScene : public Ellysia::IGameScene{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	TestScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize()override;

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="gameManager"></param>
	void Update(Ellysia::GameManager* gameManager)override;

	/// <summary>
	/// 3Dオブジェクト
	/// </summary>
	void DrawObject3D()override;

	/// <summary>
	/// ポストエフェクト描画前
	/// </summary>
	void PreDrawPostEffectFirst()override;

	/// <summary>
	/// ポストエフェクトの描画
	/// </summary>
	void DrawPostEffect()override;

	/// <summary>
	/// スプライト
	/// </summary>
	void DrawSprite()override;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~TestScene() = default;


private:
	//入力
	Ellysia::Input* input_ = nullptr;
	//モデル管理クラス
	Ellysia::ModelManager* modelManager_ = nullptr;
	//レベルエディタ
	Ellysia::LevelDataManager* levelDataManager_ = nullptr;
	//ハンドル
	uint32_t levelHandle_ = 0u;

private:
	//背景
	std::unique_ptr<Ellysia::BackTexture>backTexture_ = nullptr;

	//仮プレイヤー
	std::unique_ptr<Ellysia::Model>playerModel_ = nullptr;
	WorldTransform playerWorldTransform_ = {};
	

	//カメラ
	Camera camera_ = {};
	//平行光源
	DirectionalLight directionalLight_ = {};
	//マテリアル
	Material playerMaterial_ = {};

};