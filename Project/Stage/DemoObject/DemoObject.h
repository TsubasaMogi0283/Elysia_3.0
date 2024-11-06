#pragma once
#include "Stage/StageObject/StageObject.h"

class DemoObject : public StageObjectPre {
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
	void Draw(Camera& camera,Material& material, SpotLight& spotLight)override;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~DemoObject() = default;

public:
	/// <summary>
	/// ワールド座標
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition()override;


	AABB GetAABB() override;




};

