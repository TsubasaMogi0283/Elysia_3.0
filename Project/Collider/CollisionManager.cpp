#include "CollisionManager.h"
#include <VectorCalculation.h>

#include "AABB.h"
#include <CollisionCalculation.h>

void CollisionManager::RegisterList(Collider* collider){
	//引数から登録
	colliders_.push_back(collider);
}


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

	//各AABBの取得
	AABB aabb1 = colliderA->GetAABB();
	AABB aabb2 = colliderB->GetAABB();

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



void CollisionManager::CheckFanAndPoint(Collider* colliderA, Collider* colliderB){

	//衝突フィルタリング
	//ビット演算だから&で
	//当たらないなら計算する必要はないのでreturn
	if ((colliderA->GetCollisionAttribute() & colliderB->GetCollisionMask()) == 0 ||
		(colliderB->GetCollisionAttribute() & colliderA->GetCollisionMask()) == 0) {
		return;
	}

	
	if (colliderA->GetCollisionType() == ColliderType::PointType) {
		//衝突判定の計算
		if (CollisionCalculation::IsFanCollision(colliderB->GetFan3D(), colliderA->GetWorldPosition()) == true) {
			colliderA->OnCollision();
			colliderB->OnCollision();
		}
		else {
			colliderA->OffCollision();
			colliderB->OffCollision();
		}
	}
	else if (colliderA->GetCollisionType() == ColliderType::FanType) {
		//衝突判定の計算
		if (CollisionCalculation::IsFanCollision(colliderA->GetFan3D(), colliderB->GetWorldPosition()) == true) {
			colliderA->OnCollision();
			colliderB->OnCollision();
		}
		else {
			colliderA->OffCollision();
			colliderB->OffCollision();
		}
	}


	

}

void CollisionManager::CheckPlaneAndPoint(Collider* colliderA, Collider* colliderB){


	if ((colliderA->GetCollisionAttribute() & colliderB->GetCollisionMask()) == 0 ||
		(colliderB->GetCollisionAttribute() & colliderA->GetCollisionMask()) == 0) {
		return;
	}


	if (colliderA->GetCollisionType() == ColliderType::PointType) {
		//衝突判定の計算
		if (CollisionCalculation::IsCollisionPlaneAndPoint(colliderA->GetWorldPosition(), colliderB->GetPlane())==true) {
			colliderA->OnCollision();
			colliderB->OnCollision();
		}
		else {
			colliderA->OffCollision();
			colliderB->OffCollision();
		}
	}
	else if (colliderA->GetCollisionType() == ColliderType::PlaneType) {
		//衝突判定の計算
		if (CollisionCalculation::IsCollisionPlaneAndPoint(colliderB->GetWorldPosition(), colliderA->GetPlane())==true) {
			colliderA->OnCollision();
			colliderB->OnCollision();
		}
		else {
			colliderA->OffCollision();
			colliderB->OffCollision();
		}

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
			if (colliderA->GetCollisionType() == ColliderType::SphereType &&
				colliderB->GetCollisionType() == ColliderType::SphereType) {
				CheckSphereCollisionPair(colliderA, colliderB);
			}

			//AABB同士
			if (colliderA->GetCollisionType() == ColliderType::AABBType &&
				colliderB->GetCollisionType() == ColliderType::AABBType) {
				CheckAABBCollisionPair(colliderA, colliderB);
			}

			//扇と点
			if ((colliderA->GetCollisionType() == ColliderType::FanType && colliderB->GetCollisionType() == ColliderType::PointType)||
				(colliderA->GetCollisionType() == ColliderType::PointType && colliderB->GetCollisionType() == ColliderType::FanType)) {
				CheckFanAndPoint(colliderA, colliderB);
			}
			//平面と点
			if ((colliderA->GetCollisionType() == ColliderType::PointType && colliderB->GetCollisionType() == ColliderType::PlaneType)||
				(colliderA->GetCollisionType() == ColliderType::PlaneType && colliderB->GetCollisionType() == ColliderType::PointType)) {
				CheckPlaneAndPoint(colliderA, colliderB);
			}

		}

	}
}


void CollisionManager::ClearList(){
	colliders_.clear();
}
