#include "PushBackCalculation.h"

#include <cstdlib>
#include <numbers>

#include "CollisionCalculation.h"
#include "VectorCalculation.h"

Vector3 PushBackCalculation::AABBPushBack(const AABB& aabb1, const AABB& aabb2){
	//左に押す距離
	float dx1 = aabb2.max.x - aabb1.min.x;
	//右に押す距離
	float dx2 = aabb1.max.x - aabb2.min.x;
	//下に押す距離
	float dy1 = aabb2.max.y - aabb1.min.y;
	//上に押す距離
	float dy2 = aabb1.max.y - aabb2.min.y;
	//手前に押す距離
	float dz1 = aabb2.max.z - aabb1.min.z;
	//奥に押す距離
	float dz2 = aabb1.max.z - aabb2.min.z;

	//最小の押し戻し量を求める
	Vector3 minPush = {
		.x = (dx1 < dx2) ? dx1 : -dx2,
		.y = (dy1 < dy2) ? dy1 : -dy2,
		.z = (dz1 < dz2) ? dz1 : -dz2
	};

	Vector3 result = {};
	// 一番押し戻し量が少ない軸で処理する
	if (std::abs(minPush.x) <= std::abs(minPush.y) &&
		std::abs(minPush.x) <= std::abs(minPush.z)) {
		result = {.x = minPush.x,.y = 0.0f,.z = 0.0f };
	}
	else if (std::abs(minPush.y) <= std::abs(minPush.x) &&
		std::abs(minPush.y) <= std::abs(minPush.z)) {
		result = {.x = 0.0f,.y = minPush.y,.z = 0.0f };
	}
	else {
		result = {.x = 0.0f,.y = 0.0f,.z = minPush.z };
	}


	return result;
}

void PushBackCalculation::FixPosition(Vector3& centerPosition, AABB& mainAABB, const AABB& targetAABB) {
	if (CollisionCalculation::IsCollisionAABBPair(mainAABB, targetAABB)) {

		//押し出し距離
		Vector3 pushBack = PushBackCalculation::AABBPushBack(mainAABB, targetAABB);

		//プレイヤーの中心座標を更新
		centerPosition = VectorCalculation::Add(centerPosition, pushBack);

		//縦横高さそれぞれの幅を計算
		Vector3 mainAABBSize= VectorCalculation::Subtract(mainAABB.max,targetAABB.min);
		Vector3 newSideSize = { .x = mainAABBSize.x / 2.0f,.y = mainAABBSize.y / 2.0f,.z = mainAABBSize.z / 2.0f };

		//AABBを更新
		mainAABB.max = VectorCalculation::Add(centerPosition, newSideSize);
		mainAABB.min = VectorCalculation::Subtract(centerPosition, newSideSize);
	}

}
