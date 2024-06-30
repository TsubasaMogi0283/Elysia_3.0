#pragma once
#include <AABB.h>
#include "Fan.h"

//AABBとPointの当たり判定
bool IsCollisionAABBAndPoint(const AABB& aabb, const Vector3& point);

//扇型
bool IsFanCollision(Fan& fan,Vector3& point);