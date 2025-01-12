#pragma once
#include <memory>

#include "Model.h"
#include "WorldTransform.h"
#include "Material.h"
#include "Collider.h"

struct Camera;
struct SpotLight;

/// <summary>
/// 強敵に対してのコライダー
/// </summary>
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
	/// <param name="material"></param>
	/// <param name="spotLight"></param>
	void Draw(const Camera& camera,const Material & material,const SpotLight& spotLight);

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
	inline Vector3 GetWorldPosition()override {
		return worldTransform_.GetWorldPosition();
	};

	/// <summary>
	/// 接触
	/// </summary>
	void OnCollision()override;

	/// <summary>
	/// 非接触
	/// </summary>
	void OffCollision()override;


private:
	//モデル
	std::unique_ptr<Model> model_ = nullptr;
	//ワールドトランスフォーム
	WorldTransform worldTransform_ = {};
	//ワールド座標
	Vector3 playerWorldPosition_ = {};


};

