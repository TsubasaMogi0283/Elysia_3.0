#include "AudioObjectForLevelEditorCollider.h"

void AudioObjectForLevelEditorCollider::Initialize(){
	collisionType_ = CollisionType::PlaneType;


}

void AudioObjectForLevelEditorCollider::OnCollision()
{
}

void AudioObjectForLevelEditorCollider::OffCollision()
{
}

Vector3 AudioObjectForLevelEditorCollider::GetWorldPosition(){
	
	return objectPosition_;
}
