#include "AudioObjectForLevelEditorCollider.h"

void AudioObjectForLevelEditorCollider::Initialize(){
	collisionType_ = CollisionType::PlaneType;


}

void AudioObjectForLevelEditorCollider::OnCollision(){
	isTouch_ = true;
}

void AudioObjectForLevelEditorCollider::OffCollision(){
	isTouch_ = false;;
}

Vector3 AudioObjectForLevelEditorCollider::GetWorldPosition(){
	
	return objectPosition_;
}
