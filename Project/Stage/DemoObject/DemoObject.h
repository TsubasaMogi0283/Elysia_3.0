#pragma once
#include "Model.h"

#include "WorldTransform.h"
#include <AABB.h>

struct Camera;
struct SpotLight;
struct Material;

class DemoObject{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	DemoObject() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="modelHandle"></param>
	/// <param name="position"></param>
	void Initialize(uint32_t& modelHandle,const Vector3& position);

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
	void Draw(Camera& camera,Material& material, SpotLight& spotLight);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~DemoObject() = default;

public:
	/// <summary>
	/// 半径を取得
	/// </summary>
	/// <returns></returns>
	float GetRadius() const {
		return radius_;
	}

	/// <summary>
	/// ワールド座標
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition();


	inline AABB GetAABB() {
		return aabb_;
	}


private:
	//各オブジェクトをリストにするかも
	WorldTransform worldTransform_ = {};
	std::unique_ptr<Model>model_ = nullptr;

	float radius_ = 1.0f;


	AABB aabb_ = {};

};

