#pragma once

/**
 * @file StrongEnemy.h
 * @brief 強敵のクラス
 * @author 茂木翼
 */



#include "BaseEnemy.h"
#include "Enemy/EnemyCondition.h"
#include "StrongEnemyCollisionToPlayer.h"


#pragma region 前方宣言

/// <summary>
/// カメラ
/// </summary>
struct Camera;

/// <summary>
/// スポットライト
/// </summary>
struct SpotLight;

#pragma endregion




/// <summary>
/// 強敵
/// </summary>
class StrongEnemy : public BaseEnemy {
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	StrongEnemy() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="modelHandle">モデルハンドル</param>
	/// <param name="position">座標</param>
	/// <param name="speed">スピード</param>
	void Initialize(const uint32_t& modelHandle, const Vector3& position, const Vector3& speed)override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;


	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	/// <param name="spotLight"></param>
	void Draw(const Camera& camera,const SpotLight& spotLight)override;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~StrongEnemy()override;


public:



	/// <summary>
	/// 追跡開始距離
	/// </summary>
	/// <param name="distance"></param>
	inline void SetTrackingStartDistance(const float& distance) {
		this->trackingStartDistance_ = distance;
	}


	/// <summary>
	/// プレイヤーに対するコリジョンを取得
	/// </summary>
	/// <returns>プレイヤーに対するコリジョン</returns>
	inline StrongEnemyCollisionToPlayer* GetStrongEnemyCollisionToPlayer()const {
		return collisionToPlayer_.get();
	}
private:

	//追跡開始距離
	float trackingStartDistance_ = 0.0f;

private:
	//プレイヤーに対してのコリジョン
	std::unique_ptr<StrongEnemyCollisionToPlayer> collisionToPlayer_ = nullptr;

	
};

