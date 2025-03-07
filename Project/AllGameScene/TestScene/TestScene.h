#pragma once

#include <memory>
#include <array>

#include "IGameScene.h"
#include "BackTexture.h"
#include "Model.h"
#include "WorldTransform.h"
#include "AABB.h"
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
	/// <summary>
	/// 押し戻し処理
	/// </summary>
	/// <param name="aabb1"></param>
	/// <param name="aabb2"></param>
	/// <returns></returns>
	Vector3 PushBackProcess(const AABB& aabb1, const AABB& aabb2);

	/// <summary>
	/// 押し戻し処理
	/// </summary>
	/// <param name="mainAABB">プレイヤーなどのメイン用AABB</param>
	/// <param name="targetAABB">オブジェクトなどのAABB</param>
	void FixPosition(AABB& mainAABB,const AABB& targetAABB);

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
	const float SIZE = 1.0f;
	const Vector3 CUBE_SIZE = { .x = SIZE ,.y = SIZE ,.z = SIZE };
	AABB playerAABB_ = {};
	Vector3 playerCenterPosition_ = {};

	bool isCollision_ = false;

	//四隅
	static const uint32_t COUNER_QUANTITY_ = 4u;
	std::array<std::unique_ptr<Ellysia::Model>, COUNER_QUANTITY_>playerCounerModel_ = {};
	std::array<WorldTransform, COUNER_QUANTITY_>playerCounerWorldTransform_ = {};

	//カメラ
	Camera camera_ = {};
	//平行光源
	DirectionalLight directionalLight_ = {};
	//マテリアル
	Material playerMaterial_ = {};

};