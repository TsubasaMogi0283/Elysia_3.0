#pragma once
#include "Collider/Collider.h"

class FlashLightCollision: public Collider{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	FlashLightCollision() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~FlashLightCollision() = default;

public:
	inline void SetFan3D(Fan3D& fan3D) {
		this->fan3D_ = fan3D;
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



};

