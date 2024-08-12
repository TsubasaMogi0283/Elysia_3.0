#pragma once

#include "Collider.h"
#include <list>




class CollisionManager{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	CollisionManager()=default;

	/// <summary>
	/// リストをクリアする
	/// </summary>
	void ClearList();

	/// <summary>
	/// リストに登録
	/// </summary>
	/// <param name="collider"></param>
	void RegisterList(Collider* collider);

	/// <summary>
	/// 当たり判定のチェック
	/// </summary>
	void CheckAllCollision();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~CollisionManager()=default;

private:

	//StatePatternでやるのも良いかも

	/// <summary>
	/// 球同士
	/// </summary>
	/// <param name="colliderA"></param>
	/// <param name="colliderB"></param>
	void CheckSphereCollisionPair(Collider* colliderA, Collider* colliderB);

	/// <summary>
	/// AABB同士
	/// </summary>
	/// <param name="colliderA"></param>
	/// <param name="colliderB"></param>
	void CheckAABBCollisionPair(Collider* colliderA, Collider* colliderB);

	/// <summary>
	/// 扇と点
	/// </summary>
	/// <param name="colliderA"></param>
	/// <param name="colliderB"></param>
	void CheckFanAndPointPair(Collider* colliderA, Collider* colliderB);


private:
	std::list<Collider*>colliders_;
};

