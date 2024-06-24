#pragma once
#include <memory>
#include "Model.h"
#include "../Collider/Collider.h"
#include "WorldTransform.h"
#include "Material.h"
struct Camera;
struct SpotLight;

class LightWeapon:public Collider{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	LightWeapon()=default;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="modelHandle"></param>
	void Initialize(uint32_t modelHandle);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="playerPosition"></param>
	/// <param name="direction"></param>
	void Update(Vector3 playerPosition,Vector3 direction);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	/// <param name="spotLight"></param>
	void Draw(Camera& camera,SpotLight& spotLight);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~LightWeapon()=default;


public:
	//衝突判定
	void OnCollision()override;
	//ワールド座標を取得
	Vector3 GetWorldPosition()override;


private:
	//モデル
	std::unique_ptr<Model> model_ = nullptr;
	//ワールドトランスフォーム
	WorldTransform worldTransform_ = {};
	//マテリアル
	Material material_ = {};

	//プレイヤーとの距離
	float distanceOffset_ = 32.0f;

	//衝突
	bool isCollision_ = false;

};

