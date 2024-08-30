#include "Tree.h"

void Tree::Initialize(uint32_t& modelHandle, Vector3& position){
	//モデルの作成
	model_.reset(Model::Create(modelHandle));
	leefModel_.reset(Model::Create(leefModelHandle_));

	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	const float SIZE = 1.5f;
	worldTransform_.scale_ = { .x = SIZE ,.y = SIZE ,.z = SIZE };
	worldTransform_.translate_ = position;


	Vector3 min = {};
	Vector3 max = {};

	aabb_.min.x = position.x - min.x;
	aabb_.min.y = position.y - min.y;
	aabb_.min.z = position.z - min.z;

	aabb_.max.x = position.x + max.x;
	aabb_.max.y = position.y + max.y;
	aabb_.max.z = position.z + max.z;

}

void Tree::Update(){
	//ワールドトランスフォームの更新
	worldTransform_.Update();
}

void Tree::Draw(Camera& camera, Material& material, SpotLight& spotLight){
	model_->Draw(worldTransform_, camera, material, spotLight);
	leefModel_->Draw(worldTransform_, camera, material, spotLight);
}

Vector3 Tree::GetWorldPosition(){
	Vector3 position = {
		.x = worldTransform_.worldMatrix_.m[3][0],
		.y = worldTransform_.worldMatrix_.m[3][1],
		.z = worldTransform_.worldMatrix_.m[3][2],
	};
	return position;
}

AABB Tree::GetAABB(){
	return aabb_;
}
