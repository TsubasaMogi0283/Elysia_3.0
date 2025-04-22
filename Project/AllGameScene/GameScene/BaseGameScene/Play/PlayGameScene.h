#pragma once


/**
 * @file PlayGameScene.h
 * @brief プレイ(本編)のクラス
 * @author 茂木翼
 */

#include "CollisionManager.h"
#include "GameScene/BaseGameScene/BaseGameScene.h"
#include "Stage/Gate/Gate.h"


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
	/// ビネットの処理
	/// </summary>
	void VigntteProcess();

	/// <summary>
	/// ImGui表示用
	/// </summary>
	void DisplayImGui()override;



private:
	//回転
	const float_t ROTATE_VALUE = 0.01f;
	//コントローラーがある場合
	const float_t MOVE_LIMITATION_ = 0.02f;
	//最大値
	const float_t MAX_VIGNETTE_POW_ = 1.6f;

	//HPが1でピンチの場合
	const uint32_t DANGEROUS_HP_ = 1u;

	//警告時間
	//最大時間
	const float_t MAX_WARNING_TIME_ = 1.0f;
	//最小時間
	const float_t MIN_WARNING_TIME_ = 1.0f;
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

	//離した瞬間の状態
	bool isReleaseAttack_ = false;


	//門の回転
	float_t rightGateRotateTheta_ = 0.0f;
	float_t leftGateRotateTheta_ = 0.0f;

	//宝箱が開いているかどうか
	bool isOpenTreasureBox_ = false;
	//開けるかどうかを表示する
	std::unique_ptr<Elysia::Sprite>openTreasureBoxSprite_ = nullptr;

	//脱出テキストのスプライト
	std::unique_ptr<Elysia::Sprite> escapeText_ = nullptr;

	//ゲート
	std::unique_ptr<Gate> gate_ = nullptr;
	//脱出成功かどうか
	bool isSucceedEscape_ = false;

	//ビネットの変化時間
	float_t vignetteChangeTime_ = 0.0f;
	//警告の時間
	float_t warningTime_ = 0.0f;


};