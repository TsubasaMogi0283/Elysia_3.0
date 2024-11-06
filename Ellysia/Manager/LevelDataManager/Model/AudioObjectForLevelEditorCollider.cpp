#include "AudioObjectForLevelEditorCollider.h"
#include <Collider/CollisionConfig.h>

void AudioObjectForLevelEditorCollider::Initialize(){
	collisionType_ = CollisionType::PlaneType;

	//自分
	SetCollisionAttribute(COLLISION_ATTRIBUTE_PLAYER3);
	//相手
	SetCollisionMask(COLLISION_ATTRIBUTE_AUDIO_OBJECT);

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
