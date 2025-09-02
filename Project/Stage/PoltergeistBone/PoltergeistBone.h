#pragma once

/**
 * @file PoltergeistBone.h
 * @brief 骨のポルターガイストクラス
 * @author 茂木翼
 */

#include <Particle3D.h>
#include <Material.h>

/// <summary>
/// ElysiaEngine
/// </summary>
namespace Elysia {
	/// <summary>
	/// オーディオ
	/// </summary>
	class Audio;

	/// <summary>
	/// レベルデータ管理クラス
	/// </summary>
	class LevelDataManager;

}

/// <summary>
/// 骨のポルターガイスト
/// </summary>
class PoltergeistBone{
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
	/// 描画
	/// </summary>
	void Draw(const Camera& camera, const SpotLight& spotLight);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~PoltergeistBone() = default;
private:
	//レベルエディタ
	Elysia::LevelDataManager* levelDataManager_ = nullptr;
	//ハンドル
	uint32_t levelDataHandle_ = 0u;
	//オーディオ
	Elysia::Audio* audio_ = nullptr;
	//骨の名前
	std::string boneString_ = "Bone001";


private:
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

