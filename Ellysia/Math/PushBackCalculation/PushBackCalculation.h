#pragma once


#include "Vector3.h"
#include "AABB.h"

/// <summary>
/// 押し戻し量を計算する
/// </summary>
namespace PushBackCalculation{

	/// <summary>
	/// AABBの押し戻し量を計算
	/// </summary>
	/// <param name="aabb1">1つ目のAABB</param>
	/// <param name="aabb2">2つ目のAABB</param>
	/// <returns></returns>
	Vector3 AABBPushBack(const AABB& aabb1, const AABB& aabb2);

	void FixPosition(Vector3& centerPosition, AABB& mainAABB, const AABB& targetAABB);


};

