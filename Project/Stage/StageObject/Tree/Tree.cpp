#include "Tree.h"
#include <ModelManager.h>

void Tree::Initialize(const uint32_t& modelHandle,const Vector3& position){
	//モデルの作成
	model_.reset(Model::Create(modelHandle));
	uint32_t leefmodelHandle = ModelManager::GetInstance()->LoadModelFile("Resources/External/Model/tree_3D/Tree1", "Leef1.obj");
	leefModel_.reset(Model::Create(leefmodelHandle));

	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	const float SIZE = 1.5f;
	worldTransform_.scale = { .x = SIZE ,.y = SIZE ,.z = SIZE };
	worldTransform_.translate = position;

	const float INTERVAL = 1.0f;
	Vector3 min = { .x = INTERVAL,.y = INTERVAL,.z = INTERVAL };
	Vector3 max = { .x = INTERVAL,.y = INTERVAL,.z = INTERVAL };

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
		.x = worldTransform_.worldMatrix.m[3][0],
		.y = worldTransform_.worldMatrix.m[3][1],
		.z = worldTransform_.worldMatrix.m[3][2],
	};
	return position;
}

AABB Tree::GetAABB(){
	return aabb_;
}


