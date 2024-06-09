#pragma once

#include "Collider.h"
#include <list>


enum CollisionType {
	//球
	SphereType,
	//箱(縦横指定)
	BoxType,
};


class CollisionManager{
public:
	//コンストラクタ
	CollisionManager()=default;

	//リストをクリアする
	void ClearList();

	//リストに登録する
	void RegisterList(Collider* collider);

	//メインの当たり判定処理
	void CheckAllCollision();

	//デストラクタ
	~CollisionManager()=default;

private:
	void CheckSphereCollisionPair(Collider* colliderA, Collider* colliderB);

private:

	struct AABB {
		//奥左上
		Vector3 leftTopUp;
		//手前右下
		Vector3 rightBottomDown;
	};




private:
	std::list<Collider*>colliders_;
};

