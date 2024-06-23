#pragma once
#include "Vector3.h"
#include "WorldTransform.h"
#include "Model.h"
#include "../Collider/Collider.h"
#include "Material.h"
#include <memory>
struct Camera;
struct SpotLight;

class Enemy :public Collider{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Enemy()=default;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="modelHandle"></param>
	/// <param name="position"></param>
	/// <param name="speed"></param>
	void Initialize(uint32_t modelHandle, Vector3 position, Vector3 speed);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();


	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	/// <param name="spotLight"></param>
	void Draw(Camera& camera,SpotLight& spotLight);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Enemy()=default;



public:
	void OnCollision()override;

	Vector3 GetWorldPosition()override;

	void SetTranslate(Vector3 translate);

	void SetSammeEnemyCollisionAfter(Vector3 position, Vector3 speed);

public:
	


	bool GetIsAlive() {
		return isAlive_;
	}



private:
	WorldTransform worldTransform_ = {};
	std::unique_ptr<Model> model_ = nullptr;
	Vector4 color_ = {};
	Vector3 speed_ = {};
	Material material_ = {};


	int32_t deleteTime_ = 0;
	bool isAlive_ = true;
};

