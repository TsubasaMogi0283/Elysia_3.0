#include "PlayerCollisionToStageObject.h"
#include "CollisionConfig.h"
#include <imgui.h>

void PlayerCollisionToStageObject::Initialize(){
	collisionType_ = ColliderType::AABBType;

	//自分
	SetCollisionAttribute(COLLISION_ATTRIBUTE_PLAYER4);
	//相手
	SetCollisionMask(COLLISION_ATTRIBUTE_STAGE_OBJECT);

}

void PlayerCollisionToStageObject::Update(){

	const float SIZE = 0.5f;

	//AABBに値を入れていく
	aabb_.max = {
		.x = position_.x + SIZE,
		.y = position_.y + SIZE,
		.z = position_.z + SIZE,
	};

	aabb_.min = {
		.x = position_.x - SIZE,
		.y = position_.y - SIZE,
		.z = position_.z - SIZE,
	};




#ifdef _DEBUG
	ImGui::Begin("プレイヤーのコリジョン(ステージオブジェクト用)");
	ImGui::Checkbox("接触", &isTouch_);
	ImGui::InputFloat3("位置", &position_.x);
	ImGui::InputFloat3("MAX", &aabb_.max.x);
	ImGui::InputFloat3("MIN", &aabb_.min.x);
	ImGui::End();
#endif // _DEBUG


}

void PlayerCollisionToStageObject::OnCollision(){
	isTouch_ = true;
}

void PlayerCollisionToStageObject::OffCollision(){
	isTouch_ = false;
}

Vector3 PlayerCollisionToStageObject::GetWorldPosition(){

	return Vector3();
}
