#pragma once


/**
 * @file PlayGameScene.h
 * @brief プレイ(本編)のクラス
 * @author 茂木翼
 */

#include "CollisionManager.h"
#include "GameScene/BaseGameScene/BaseGameScene.h"



/// <summary>
/// プレイ(本編)
/// </summary>
class PlayGameScene : public BaseGameScene {
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	PlayGameScene();

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
	~PlayGameScene()override=default;

private:
	/// <summary>
	/// コリジョン管理クラスに登録
	/// </summary>
	void RegisterToCollisionManager();

	/// <summary>
	/// プレイヤーの移動
	/// </summary>
	void PlayerMove();

	/// <summary>
	/// プレイヤーの回転
	/// </summary>
	void PlayerRotate();

	/// <summary>
	/// 逃げ状態
	/// </summary>
	void EscapeCondition();

	/// <summary>
	/// ステージオブジェクトとの当たり判定
	/// </summary>
	void ObjectCollision();

	/// <summary>
	/// ImGui表示用
	/// </summary>
	void DisplayImGui()override;

private:

	//コリジョン管理
	std::unique_ptr<Elysia::CollisionManager> collisionManager_ = nullptr;

	//UIの表示
	bool isDisplayUI_ = false;

	//角度
	float_t theta_ = 0.0f;
	float_t phi_ = 0.0f;

	//柵の座標
	Vector3 fenceTranslate_ = {};

};