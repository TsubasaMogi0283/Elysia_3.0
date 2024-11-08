#pragma once

#include "Collider/Collider.h"


/// <summary>
/// レベルエディタ用のオブジェクトの当たり判定
/// </summary>
class IObjectForLevelEditorCollider:public Collider {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize()=0;

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// オブジェクトの座標を取得
	/// </summary>
	/// <param name="position"></param>
	virtual void SetObjectPosition(const Vector3& position) {
		this->objectPosition_ = position;
	};

	/// <summary>
	/// 衝突したかどうかのフラグを取得
	/// </summary>
	/// <returns></returns>
	virtual bool GetIsTouch() const{
		return isTouch_;
	}

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~IObjectForLevelEditorCollider() = default;


protected:
	//オブジェクトの座標
	Vector3 objectPosition_ = {};

	//衝突したかどうか
	bool isTouch_ = false;



};