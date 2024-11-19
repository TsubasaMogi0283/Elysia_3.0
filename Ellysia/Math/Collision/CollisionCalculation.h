#pragma once
#include "AABB.h"
#include "Fan.h"
#include "Plane.h"
#include "Vector2.h"
#include "Vector3.h"

/// <summary>
/// 衝突判定の計算
/// </summary>
namespace CollisionCalculation {

	/// <summary>
	/// AABBとPointの当たり判定
	/// </summary>
	/// <param name="aabb"></param>
	/// <param name="point"></param>
	/// <returns></returns>
	bool IsCollisionAABBAndPoint(const AABB& aabb, const Vector3& point);

	/// <summary>
	/// 平面と点
	/// </summary>
	/// <param name="position"></param>
	/// <param name="plane"></param>
	/// <returns></returns>
	bool IsCollisionPlaneAndPoint(const Vector3& position, const Plane& plane);

	/// <summary>
	/// 扇形(2次元)
	/// </summary>
	/// <param name="fan"></param>
	/// <param name="point"></param>
	/// <returns></returns>
	bool IsFanCollision(const Fan2D& fan, const Vector2& point);

	/// <summary>
	/// 扇形(3次元)
	/// </summary>
	/// <param name="fan"></param>
	/// <param name="point"></param>
	/// <returns></returns>
	bool IsFanCollision(const Fan3D& fan, const Vector3& point);

}


