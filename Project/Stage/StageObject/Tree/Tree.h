#pragma once

#include "Stage/StageObject/StageObject.h"


class Tree :public StageObject{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Tree() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="modelHandle"></param>
	/// <param name="position"></param>
	void Initialize(const uint32_t& modelHandle,const Vector3& position) override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	/// <param name="material"></param>
	/// <param name="spotLight"></param>
	void Draw(Camera& camera, Material& material, SpotLight& spotLight) override;


	/// <summary>
	/// デストラクタ
	/// </summary>
	~Tree() =default;


public:


	/// <summary>
	/// ワールド座標を取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition();

	/// <summary>
	/// AABBを取得
	/// </summary>
	/// <returns></returns>
	AABB GetAABB();





private:

	//StageObjectは基本1個のモデルだけどこれのクラスでは例外で+1個必要
	std::unique_ptr<Model> leefModel_ = nullptr;
};

