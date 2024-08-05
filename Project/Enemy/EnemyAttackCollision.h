#pragma once
#include <memory>

#include "Model.h"
#include "WorldTransform.h"
#include "Material.h"
#include "../Collider/Collider.h"

struct Camera;
struct SpotLight;

class EnemyAttackCollision :public Collider {
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	EnemyAttackCollision() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="modelHandle"></param>
	void Initialize(uint32_t modelHandle);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	/// <param name="spotLight"></param>
	void Draw(Camera&camera,SpotLight& spotLight);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~EnemyAttackCollision() = default;


public:
	/// <summary>
	/// 敵のワールド座標の設定
	/// </summary>
	/// <param name="position"></param>
	inline void SetEnemyPosition(Vector3& position) {
		this->enemyWorldPosition_ = position;
	}

	/// <summary>
	/// 敵の方向を設定
	/// </summary>
	/// <param name="direction"></param>
	inline void SetEnemyDirection(Vector3& direction) {
		this->enemyDirection_ = direction;
	}

	/// <summary>
	/// ワールド座標の取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition()override;

	/// <summary>
	/// 衝突
	/// </summary>
	void OnCollision()override;

private:
	std::unique_ptr<Model> model_ = nullptr;
	WorldTransform worldTransform_ = {};
	Material material_ = {};

	Vector3 enemyWorldPosition_ = {};
	Vector3 enemyDirection_ = {};
};

