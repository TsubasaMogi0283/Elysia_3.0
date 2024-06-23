#pragma once
#include <memory>
#include "Model.h"
#include "../Collider/Collider.h"
#include "WorldTransform.h"
struct Camera;
struct SpotLight;
struct Material;

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
	/// <param name="cameraPosition"></param>
	void Update(Vector3 cameraPosition);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	/// <param name="material"></param>
	/// <param name="spotLight"></param>
	void Draw(Camera& camera,Material& material,SpotLight& spotLight);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~LightWeapon()=default;


public:
	void OnCollision()override;

	Vector3 GetWorldPosition()override;



private:
	std::unique_ptr<Model> model_ = nullptr;
	WorldTransform worldTransform_ = {};

	Vector4 color_ = {};
	Vector4 lightColor_ = {};
	float DISTANCE_OFFSET = 32.0f;

	bool isCollision_ = false;

};

