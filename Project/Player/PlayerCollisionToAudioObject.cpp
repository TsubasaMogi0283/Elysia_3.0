#include "PlayerCollisionToAudioObject.h"
#include <Collider/CollisionConfig.h>
#include <imgui.h>

void PlayerCollisionToAudioObject::Initialize(){
	//点の方が良いかも
	collisionType_ = CollisionType::PointType;

	//自分
	SetCollisionAttribute(COLLISION_ATTRIBUTE_PLAYER3);
	//相手
	SetCollisionMask(COLLISION_ATTRIBUTE_AUDIO_OBJECT);



}

void PlayerCollisionToAudioObject::Update(){

#ifdef _DEBUG
	ImGui::Begin("当たり判定(オーディオオブジェクト用)");
	ImGui::Checkbox("衝突",&isTouch_);
	ImGui::InputFloat3("座標", &position_.x);
	ImGui::End();
#endif // _DEBUG



}

void PlayerCollisionToAudioObject::OnCollision(){
	isTouch_ = true;
}

void PlayerCollisionToAudioObject::OffCollision(){
	isTouch_ = false;
}

Vector3 PlayerCollisionToAudioObject::GetWorldPosition(){
	return position_;
}
