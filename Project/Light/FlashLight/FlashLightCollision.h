#pragma once
#include "Collider/Collider.h"

class FlashLightCollision: public Collider{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	FlashLightCollision() = default;

	void Initialize();


	/// <summary>
	/// デストラクタ
	/// </summary>
	~FlashLightCollision() = default;

public:
	inline Fan3D SetFan3D(Fan3D& fan3D) {
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




private:


};

