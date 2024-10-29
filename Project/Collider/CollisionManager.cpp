#include "CollisionManager.h"
#include <VectorCalculation.h>

#include "AABB.h"
#include <Collision.h>

void CollisionManager::RegisterList(Collider* collider){
	//引数から登録
	colliders_.push_back(collider);
}

//コライダー2つの衝突判定と応答
void CollisionManager::CheckSphereCollisionPair(Collider* colliderA, Collider* colliderB) {

	//コライダーAのワールド座標を取得
	Vector3 colliderPositionA = colliderA->GetWorldPosition();
	
	//コライダーBのワールド座標を取得
	Vector3 colliderPositionB = colliderB->GetWorldPosition();
	
	//衝突フィルタリング
	//ビット演算だから&で
	//当たらないなら計算する必要はないのでreturn
	if ((colliderA->GetCollisionAttribute() & colliderB->GetCollisionMask()) == 0 ||
		(colliderB->GetCollisionAttribute() & colliderA->GetCollisionMask()) == 0) {
		return;
	}




#pragma region Sphereの当たり判定を計算

	//AとBの差分ベクトルを求める
	Vector3 difference = VectorCalculation::Subtract(colliderPositionA, colliderPositionB);

	//距離を計算
	float distance = sqrtf((difference.x * difference.x) + 
		(difference.y * difference.y) + 
		(difference.z * difference.z));
	
	//当たった時
	if (distance <= colliderA->GetRadius() + colliderB->GetRadius()) {
		colliderA->OnCollision();
		colliderB->OnCollision();
	}
	else {
		colliderA->OffCollision();
		colliderB->OffCollision();
	}

#pragma endregion

}

void CollisionManager::CheckAABBCollisionPair(Collider* colliderA, Collider* colliderB){

	//衝突フィルタリング
	//ビット演算だから&で
	//当たらないなら計算する必要はないのでreturn
	if ((colliderA->GetCollisionAttribute() & colliderB->GetCollisionMask()) == 0 ||
		(colliderB->GetCollisionAttribute() & colliderA->GetCollisionMask()) == 0) {
		return;
	}


	//コライダーAのワールド座標を取得
	Vector3 colliderPosisionA = colliderA->GetWorldPosition();

	//コライダーBのワールド座標を取得
	Vector3 colliderPosisionB = colliderB->GetWorldPosition();

	

	//コライダーAの下方サイズ
	Vector3 downSideSizeA = colliderA->GetDownSideSize();
	//コライダーBの下方サイズ
	Vector3 downSideSizeB = colliderB->GetDownSideSize();


	//コライダーAの上方サイズ
	Vector3 upSideSizeA = colliderA->GetUpSideSize();
	//コライダーBの上方サイズ
	Vector3 upSideSizeB = colliderB->GetUpSideSize();



	//コライダーAのAABBを求める
	AABB aabb1 = {
		.min{.x = colliderPosisionA.x - downSideSizeA.x,.y = colliderPosisionA.y - downSideSizeA.y,.z = colliderPosisionA.z - downSideSizeA.z },
		.max{.x = colliderPosisionA.x + upSideSizeA.x,.y = colliderPosisionA.y + upSideSizeA.y,.z = colliderPosisionA.z + upSideSizeA.z }
	};

	//コライダーBのAABBを求める
	AABB aabb2 = {
		.min{.x = colliderPosisionB.x - downSideSizeB.x,.y = colliderPosisionB.y - downSideSizeB.y,.z = colliderPosisionB.z - downSideSizeB.z },
		.max{.x = colliderPosisionB.x + upSideSizeB.x,.y = colliderPosisionB.y + upSideSizeB.y,.z = colliderPosisionB.z + upSideSizeB.z }
	};


	//衝突判定
	if ((aabb1.min.x <= aabb2.max.x && aabb1.max.x >= aabb2.min.x) &&
		(aabb1.min.y <= aabb2.max.y && aabb1.max.y >= aabb2.min.y) &&
		(aabb1.min.z <= aabb2.max.z && aabb1.max.z >= aabb2.min.z)) {
		colliderA->OnCollision();
		colliderB->OnCollision();
	}
	else {
		colliderA->OffCollision();
		colliderB->OffCollision();
	}

}



void CollisionManager::CheckFanAndPointPair(Collider* colliderA, Collider* colliderB){

	//衝突フィルタリング
	//ビット演算だから&で
	//当たらないなら計算する必要はないのでreturn
	if ((colliderA->GetCollisionAttribute() & colliderB->GetCollisionMask()) == 0 ||
		(colliderB->GetCollisionAttribute() & colliderA->GetCollisionMask()) == 0) {
		return;
	}

	//衝突判定
	if (IsFanCollision(colliderA->GetFan3D(), colliderB->GetWorldPosition())) {
		colliderA->OnCollision();
		colliderB->OnCollision();
	}
	else {
		colliderA->OffCollision();
		colliderB->OffCollision();
	}

}


void CollisionManager::CheckAllCollision(){


	//総当たりの判定
	std::list<Collider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA) {
		
		//itrAの*は「参照」
		//itrAを全通りcolliderAに登録しているよ
		Collider* colliderA = *itrA;

		//イテレータBはイテレータAの次の要素から回す(重複判定を回避)
		std::list<Collider*>::iterator itrB = itrA;
		itrB++;

		for (; itrB != colliders_.end(); ++itrB) {
			Collider* colliderB = *itrB;

			//球同士
			if (colliderA->GetCollisionType() == CollisionType::SphereType &&
				colliderB->GetCollisionType() == CollisionType::SphereType) {
				CheckSphereCollisionPair(colliderA, colliderB);
			}

			//AABB同士
			if (colliderA->GetCollisionType() == CollisionType::AABBType &&
				colliderB->GetCollisionType() == CollisionType::AABBType) {
				CheckAABBCollisionPair(colliderA, colliderB);
			}

			//扇と点
			if (colliderA->GetCollisionType() == CollisionType::FanType &&
				colliderB->GetCollisionType() == CollisionType::PointType) {
				CheckFanAndPointPair(colliderA, colliderB);
			}
			

		}

	}
}

//試しにclear抜いてみたら動作が凄く重くなった
void CollisionManager::ClearList(){
	colliders_.clear();
}
