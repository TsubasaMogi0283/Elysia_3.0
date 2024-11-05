#include "StageObjectForLevelEditorCollider.h"

void StageObjectForLevelEditorCollider::Initialize() {
	collisionType_ = CollisionType::PlaneType;

	//プレイヤー(レベルエディタのステージオブジェクト)
	const uint32_t COLLISION_ATTRIBUTE_PLAYER4 = 0b1 << 9;

	//ステージオブジェクト(レベルエディタ)
	const uint32_t COLLISION_ATTRIBUTE_STAGE_OBJECT = 0b1 << 10;

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
