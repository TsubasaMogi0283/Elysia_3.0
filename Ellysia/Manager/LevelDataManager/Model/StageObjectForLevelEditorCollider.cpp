#include "StageObjectForLevelEditorCollider.h"
#include <Collider/CollisionConfig.h>

void StageObjectForLevelEditorCollider::Initialize() {
	collisionType_ = CollisionType::AABBType;

	
	//自分
	SetCollisionAttribute(COLLISION_ATTRIBUTE_STAGE_OBJECT);
	//相手
	SetCollisionMask(COLLISION_ATTRIBUTE_PLAYER4);



}

void StageObjectForLevelEditorCollider::OnCollision(){
	isTouch_ = true;
}

void StageObjectForLevelEditorCollider::OffCollision(){
	isTouch_ = false;
}

Vector3 StageObjectForLevelEditorCollider::GetWorldPosition() {
	return objectPosition_;
}
