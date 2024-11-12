#include "StageObjectForLevelEditorCollider.h"
#include <Collider/CollisionConfig.h>
#include <imgui.h>

void StageObjectForLevelEditorCollider::Initialize() {
	//基本AABB
	//必要があれば追加していく
	collisionType_ = ColliderType::AABBType;

	

	

	//自分
	SetCollisionAttribute(COLLISION_ATTRIBUTE_STAGE_OBJECT);
	//相手
	SetCollisionMask(COLLISION_ATTRIBUTE_PLAYER4);

}

void StageObjectForLevelEditorCollider::Update(){
	

#ifdef _DEBUG
	ImGui::Begin("ステージオブジェクト");
	ImGui::InputFloat3("位置", &objectPosition_.x);
	ImGui::Checkbox("衝突", &isTouch_);
	ImGui::End();
#endif // _DEBUG

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
