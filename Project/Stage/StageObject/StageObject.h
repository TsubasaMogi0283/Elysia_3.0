#pragma once
#include <memory>

#include "Model.h"
#include <WorldTransform.h>
#include <AABB.h>

struct Camera;
struct Material;
struct SpotLight;

class StageObject{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="modelHandle"></param>
	/// <param name="position"></param>
	virtual void Initialize(uint32_t& modelHandle,Vector3& position) = 0;

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	/// <param name="material"></param>
	/// <param name="spotLight"></param>
	virtual void Draw(Camera& camera, Material& material, SpotLight& spotLight)=0;


public:

	/// <summary>
	/// ワールド座標を取得
	/// </summary>
	/// <returns></returns>
	virtual Vector3 GetWorldPosition()=0;

	/// <summary>
	/// AABBを取得
	/// </summary>
	/// <returns></returns>
	virtual AABB GetAABB() = 0;
	

protected:
	//ワールドトランスフォーム
	WorldTransform worldTransform_ = {};
	//モデル
	std::unique_ptr<Model>model_ = nullptr;


	//AABB
	AABB aabb_ = {};
};

