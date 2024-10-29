#include "StageObjectForLevelEditorCollider.h"

void StageObjectForLevelEditorCollider::Initialize() {
	collisionType_ = CollisionType::PlaneType;


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
