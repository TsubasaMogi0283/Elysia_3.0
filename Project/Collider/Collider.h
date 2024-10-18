#pragma once
#include "Vector3.h"
#include <cstdint>
#include <string>
#include "AABB.h"
#include <Fan.h>

enum CollisionType {
	//点
	PointType,
	//球
	SphereType,
	//AABB
	AABBType,
	//ボックス
	BoxType,
	//扇
	FanType,

};

class Collider{
public:
	//衝突時に呼ばれる関数
	virtual void OnCollision()=0;


	//ワールド座標を取得
	//純粋仮想関数
	virtual Vector3 GetWorldPosition() = 0;


public:

	//半径を取得
	float GetRadius() {
		return radius_;
	}
	//半径を設定
	void SetRadius_(float radius) {
		this->radius_ = radius;
	}



#pragma region AABB

	/// <summary>
	/// 上方のサイズ
	/// </summary>
	/// <returns></returns>
	inline Vector3 GetUpSideSize() {
		return upSideSize_;
	}
	
	/// <summary>
	/// 下方のサイズ
	/// </summary>
	/// <returns></returns>
	inline Vector3 GetDownSideSize() {
		return downSideSize_;
	}

#pragma endregion


	//3D
	inline Fan3D GetFan3D () { 
		return fan3D_;
	};

	//2D
	inline Fan2D GetFan2D() {
		return fan2D_;
	};

	/// <summary>
	/// 衝突判定で使う形の種類の値を取得
	/// </summary>
	/// <returns></returns>
	inline uint32_t GetCollisionType() {
		return collisionType_;
	}


public:
	//衝突属性(自分)を取得
	uint32_t GetCollisionAttribute() {
		return collisionAttribute_;
	}
	//衝突属性(自分)を設定
	void SetCollisionAttribute(uint32_t collisionAttribute) {
		this->collisionAttribute_ = collisionAttribute;
	}

	//衝突マスク(相手)を取得
	uint32_t GetCollisionMask() {
		return collisionMask_;
	}
	//衝突マスク(相手)を設定
	void SetCollisionMask(uint32_t collisionMask) {
		this->collisionMask_ = collisionMask;
	}

protected:

	//当たり判定の種類
	uint32_t collisionType_ = CollisionType::SphereType;

#pragma region 球

	//衝突半径
	float radius_ = 1.0f;

#pragma endregion



#pragma region AABB

	//AABBのmax部分に加算する縦横高さのサイズ
	Vector3 upSideSize_ = {};

	//AABBのmax部分に加算する縦横高さのサイズ
	Vector3 downSideSize_ = {};



#pragma endregion




#pragma region 扇
	//3D
	Fan3D fan3D_ = {};

	//2D
	Fan2D fan2D_ = {};


#pragma endregion




private:
	
	//衝突判定(自分)
	uint32_t collisionAttribute_  = 0xffffffff;

	//衝突判定(相手)
	uint32_t collisionMask_ = 0xffffffff;

};

