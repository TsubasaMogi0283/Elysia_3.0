#pragma once
#include <memory>

#include "Model.h"
#include "WorldTransform.h"
#include "Material.h"
#include "../Collider/Collider.h"

struct Camera;
struct SpotLight;


class PlayerCollisionToStrongEnemy :public Collider {

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	PlayerCollisionToStrongEnemy() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="modelHandle"></param>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	/// <param name="spotLight"></param>
	void Draw(Camera& camera, SpotLight& spotLight);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~PlayerCollisionToStrongEnemy() = default;


public:
	/// <summary>
	/// 敵のワールド座標の設定
	/// </summary>
	/// <param name="position"></param>
	inline void SetPlayerPosition(Vector3 position) {
		this->playerWorldPosition_ = position;
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

	Vector3 playerWorldPosition_ = {};


};

