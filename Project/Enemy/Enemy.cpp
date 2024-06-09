#include "Enemy.h"

void Enemy::Initialize(uint32_t modelHandle, Vector3 position){
	
	model_.reset(Model::Create(modelHandle));
	worldTransform_.Initialize();
	worldTransform_.translate_ = position;
}

void Enemy::Update(){
	
	//更新
	worldTransform_.Update();
}

void Enemy::Draw(Camera& camera){
	
	//描画
	model_->Draw(worldTransform_, camera);
}
