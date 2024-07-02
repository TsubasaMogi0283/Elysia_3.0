#pragma once
#include <AABB.h>
#include "Fan.h"
#include <Vector2.h>

//AABBとPointの当たり判定
bool IsCollisionAABBAndPoint(const AABB& aabb, const Vector3& point);

//扇型
bool IsFanAndPointCollision(const Fan& fan,const Vector2& point);