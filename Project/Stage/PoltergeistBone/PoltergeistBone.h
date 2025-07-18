#pragma once

/**
 * @file GameManager.h
 * @brief ドア(レベルエディタ)の処理用クラス
 * @author 茂木翼
 */

#include <memory>
#include <Vector3.h>
#include <Particle3D.h>
#include <Material.h>

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
	/// モデル管理クラス
	/// </summary>
	class ModelManager;

	/// <summary>
	/// オーディオ
	/// </summary>
	class Audio;


}

/// <summary>
/// プレイヤー
/// </summary>
class Player;

/// <summary>
/// 鍵管理クラス
/// </summary>
class KeyManager;

/// <summary>
/// ドア(レベルエディタ)
/// </summary>
class PoltergeistBone {
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	PoltergeistBone();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 3Dモデルの描画
	/// </summary>
	/// <param name="camera">カメラ</param>
	/// <param name="spotLight">スポットライト</param>
	void Draw(const Camera& camera, const SpotLight& spotLight);
	
	/// <summary>
	/// デストラクタ
	/// </summary>
	~PoltergeistBone() = default;

public:
	/// <summary>
	/// レベルデータのハンドルを取得
	/// </summary>
	/// <param name="levelDataHandle">ハンドル</param>
	inline void SetLevelDataHandle(const uint32_t& levelDataHandle) {
		this->levelDataHandle_ = levelDataHandle;
	}
	/// <summary>
	/// プレイヤーの設定
	/// </summary>
	/// <param name="player">プレイヤー</param>
	inline void SetPlayer(Player* player) {
		this->player_ = player;
	}

	/// <summary>
	/// 鍵管理クラスの設定
	/// </summary>
	/// <param name="keyManager"></param>
	inline void SetKeyManager(KeyManager* keyManager) {
		this->keyManager_ = keyManager;
	}


private:
	
	//入力クラス
	Elysia::Input* input_ = nullptr;
	//レベルエディタ
	Elysia::LevelDataManager* levelDataManager_ = nullptr;
	//モデル管理クラス
	Elysia::ModelManager* modelManager_ = nullptr;
	//オーディオ
	Elysia::Audio* audio_ = nullptr;
	//ハンドル
	uint32_t levelDataHandle_ = 0u;
	//プレイヤー
	Player* player_ = nullptr;
	//鍵管理クラス
	KeyManager* keyManager_ = nullptr;

private:
	//時間変化
	const float_t DELTA_TIME_ = 1.0f / 60.0f;

	//線形補間の最小値
	const float_t MIN_T_VALUE_ = 0.0f;
	//線形補間の最大値
	const float_t MAX_T_VALUE_ = 1.0f;
	//回転
	const float_t ROTATE_VALUE_ = 0.01f;

	//骨の名前
	std::string boneString_ = "Bone001";
	//浮遊の高さ
	const float_t FLOATING_HEIGHT_ = 4.0f;
	//浮遊時間
	const float_t FLOATING_TIME_ = 3.0f;
	//落下準備時間
	const float_t READY_FOR_DROP_TIME_ = 2.5f;
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
private:
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

	//骨が壊れる音
	uint32_t boneBreakAudioHandle_ = 0u;

	//骨の警告音
	uint32_t warningBoneAudioHandle_ = 0u;
	float_t warningFrequencyRatio_ = 1.0f;

};