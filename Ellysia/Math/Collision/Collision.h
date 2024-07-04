#pragma once
#include <AABB.h>
#include "Fan.h"
#include <Vector2.h>

//AABBとPointの当たり判定
bool IsCollisionAABBAndPoint(const AABB& aabb, const Vector3& point);

/// <summary>
/// 扇形(2次元)
/// </summary>
/// <param name="fan"></param>
/// <param name="point"></param>
/// <returns></returns>
bool IsFanCollision(const Fan2D& fan,const Vector2& point);

/// <summary>
/// 扇形(3次元)
/// </summary>
/// <param name="fan"></param>
/// <param name="point"></param>
/// <returns></returns>
bool IsFanCollision(const Fan3D& fan, const Vector3& point);

