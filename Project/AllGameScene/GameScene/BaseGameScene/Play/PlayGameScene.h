#pragma once


/**
 * @file PlayGameScene.h
 * @brief プレイ(本編)のクラス
 * @author 茂木翼
 */

#include <numbers>

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
	/// ImGui表示用
	/// </summary>
	void DisplayImGui()override;



private:
	//回転
	const float_t ROTATE_VALUE = 0.01f;
	//回転用の線形補間
	const float_t OPEN_T_VALUE_ = 0.01f;
	//フェードの増加分
	const float_t FADE_AMOUNT_ = 0.01f;

	//最大の回転
	const float_t MAX_OPEN_VALUE_ = std::numbers::pi_v<float_t> / 2.0f;

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

	//レベルエディタのオブジェクトの名前
	std::string right = "GateDoorRight";
	std::string left = "GateDoorLeft";


	//最大音量
	const float_t MAX_VOLUME_ = 0.6f;
	//最小音量
	const float_t MIN_VOLUME_ = 0.0f;
	//音量の減る値
	const float_t VOLUME_DECREASE_VALUE_ = 0.0005f;
	
private:

	//コリジョン管理
	std::unique_ptr<Elysia::CollisionManager> collisionManager_ = nullptr;

	//白フェードのスプライト
	std::unique_ptr<Elysia::Sprite> whiteFadeSprite_ = nullptr;
	//黒フェードのスプライト
	std::unique_ptr<Elysia::Sprite> blackFadeSprite_ = nullptr;
	//操作のスプライト
	std::unique_ptr<Elysia::Sprite> operationSprite_ = nullptr;


	//透明度
	float_t fadeTransparency_ = 0.0f;

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
	//線形補間
	float_t openT_ = 0.0f;

	//脱出テキストのスプライト
	std::unique_ptr<Elysia::Sprite> escapeTextSprite_ = nullptr;
	//脱出のスプライト
	std::unique_ptr<Elysia::Sprite> toEscapeSprite_ = nullptr;

	//ゲート
	std::unique_ptr<Gate> gate_ = nullptr;
	//脱出成功かどうか
	bool isSucceedEscape_ = false;

	//ビネットの変化時間
	float_t vignetteChangeTime_ = 0.0f;
	//警告の時間
	float_t warningTime_ = 0.0f;

	//環境音の音量
	float_t enviromentAudioVolume_ = 0u;


};