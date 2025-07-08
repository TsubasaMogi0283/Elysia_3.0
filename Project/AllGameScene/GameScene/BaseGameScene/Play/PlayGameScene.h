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
#include <Particle3D.h>

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
	/// 3Dモデルの描画
	/// </summary>
	/// <param name="camera">カメラ</param>
	/// <param name="spotLight">スポットライト</param>
	void DrawObject3D(const Camera& camera, const SpotLight& spotLight)override;

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
	/// 門への案内
	/// </summary>
	void EscapeAssist();

	/// <summary>
	/// 墓場の処理
	/// </summary>
	void CemeteryProcess();

	/// <summary>
	/// ポルターガイストの処理
	/// </summary>
	void PoltergeistProcess();

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
	std::string gateRightString_ = "GateDoorRight";
	std::string gateLeftString_ = "GateDoorLeft";

	//骨の名前
	std::string boneString_ = "Bone001";
	//浮遊の高さ
	const float_t FLOATING_HEIGHT_ = 4.0f;
	//浮遊時間
	const float_t FLOATING_TIME_ = 3.0f;
	//落下準備時間
	const float_t READY_FOR_DROP_TIME_=2.5f;
	//落下時間
	const float_t DROP_TIME_ = 2.0f;
	//欠片の大きさ
	const float_t BONE_PIECE_SCALE_ = 0.5f;
	//Y方向のベロシティ
	const float_t THROW_UP_VELOCITY_Y_ = 0.3f;

	//回転の値
	const float_t ROTATE_THETA_VALUE_ = 0.1f;
	//高速回転
	const float_t RAPID_ROTATE_THETA_VALUE_ = 0.5f;
	//地面の座標
	const float_t GROUND_POSITION_Y = 0.0f;

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

	//骨が上がる
	bool isBoneRise_ = true;
	//骨が上がり切ったかどうか
	bool isFinishRiseBone_ = false;
	//浮遊時間
	float_t floatingBoneTime_ = 0.0f;
	//浮遊時に使うθ
	float_t floatingTheta_ = 0.0f;
	//骨の座標
	Vector3 bonePosition_ = {};
	//落下準備
	bool isReadyForBoneDrop_ = false;
	//落下準備委時間
	float_t readyForDropTime_ = 0.0f;
	//落下
	bool isBoneDrop_ = false;
	//落下スピード
	float_t dropSpeed_ = 0.4f;
	//プレイヤーの座標ロックオン
	Vector3 loclOnPlayerPosition_ = {};
	//ロックオンしたかどうか
	bool isLockOn_ = false;
	//落下用の線形補間
	float_t dropT_ = 0.0f;
	//落下前の骨の座標
	Vector3 beforeBoneDropPosition_ = {};
	//骨とプレイヤーの方向
	Vector3 boneDirectionToPlayer_ = {};

	//骨の欠片のパーティクル
	std::unique_ptr<Elysia::Particle3D> bonePieceParticle_ = nullptr;
	//欠片のモデルハンドル
	uint32_t bonePieceParticleHandle_ = 0u;
	//骨の欠片パーティクル用のマテリアル
	Material bonePieceMaterial_ = {};
	//欠片の数
	uint32_t bonePieceCount_ = 30u;
	//一回だけ当たったかどうか
	bool isTouchOnce_ = false;

	//脱出アシスト用の矢印
	std::unique_ptr<Elysia::Model>assistArrowModel_ = nullptr;
	WorldTransform assistArrowWorldTransform_ = {};
	Material assistArrowMaterial_ = {};

	//ゲート
	std::unique_ptr<Gate> gate_ = nullptr;
	//脱出成功かどうか
	bool isSucceedEscape_ = false;

	//門を開ける音
	uint32_t openGateAudioHandle_ = 0u;
	//一回だけ鳴らす
	bool isPlayOpenSE_ = false;
	//門が閉まる音
	uint32_t closeGateAudioHandle_ = 0u;
	//一回だけ鳴らす
	bool isPlayCloseSE_ = false;
	//骨が壊れる音
	uint32_t boneBreakAudioHandle_ = 0u;

	//骨の警告音
	uint32_t warningBoneAudioHandle_ = 0u;
	float_t warningFrequencyRatio_ = 1.0f;

	//ビネットの変化時間
	float_t vignetteChangeTime_ = 0.0f;
	//警告の時間
	float_t warningTime_ = 0.0f;

	//環境音の音量
	float_t enviromentAudioVolume_ = 0u;


};