#pragma once

#include "Collider/Collider.h"


//レベルエディタ用のモデル
class IObjectForLevelEditorCollider:public Collider {
public:

	/// <summary>
	/// オブジェクトの座標を取得
	/// </summary>
	/// <param name="position"></param>
	virtual void SetObjectPosition(const Vector3& position) {
		this->objectPosition_ = position;
	};



	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~IObjectForLevelEditorCollider() = default;


protected:
	//オブジェクトの座標
	Vector3 objectPosition_ = {};





};