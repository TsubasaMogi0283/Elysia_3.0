#include "FlashLightCollision.h"
#include <Collider/CollisionConfig.h>

void FlashLightCollision::Initialize(){


#pragma region 当たり判定

	//種類
	collisionType_ = CollisionType::FanType;




	//自分
	SetCollisionAttribute(COLLISION_ATTRIBUTE_FLASH_LIGHT);
	//相手
	SetCollisionMask(COLLISION_ATTRIBUTE_ENEMY2);

#pragma endregion


}

void FlashLightCollision::Update()
{
}

Vector3 FlashLightCollision::GetWorldPosition()
{
	return Vector3();
}

void FlashLightCollision::OnCollision(){



}

void FlashLightCollision::OffCollision()
{
}
