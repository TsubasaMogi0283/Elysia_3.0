#include "AudioObjectForLevelEditorCollider.h"
#include <Collider/CollisionConfig.h>
#include <imgui.h>

void AudioObjectForLevelEditorCollider::Initialize(){
	collisionType_ = ColliderType::PlaneType;



	//自分
	SetCollisionAttribute(COLLISION_ATTRIBUTE_AUDIO_OBJECT);
	//相手
	SetCollisionMask(COLLISION_ATTRIBUTE_PLAYER3);

}


void AudioObjectForLevelEditorCollider::Plane(){
	plane_ = {
		.position = objectPosition_,
		.length = 5.0f,
		.width = 5.0f,
	};


#ifdef _DEBUG
	ImGui::Begin("AudioObjectCollider");
	ImGui::InputFloat3("位置", &plane_.position.x);
	ImGui::InputFloat("縦", &plane_.length);
	ImGui::InputFloat("横", &plane_.width);
	ImGui::Checkbox("isTouch", &isTouch_);
	ImGui::End();
#endif // _DEBUG


}

void AudioObjectForLevelEditorCollider::Update(){
	switch (collisionType_) {




	}

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


