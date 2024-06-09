#include "Enemy.h"

#include "../Collider/CollisionConfig.h"

void Enemy::Initialize(uint32_t modelHandle, Vector3 position){
	
	model_.reset(Model::Create(modelHandle));
	worldTransform_.Initialize();
	worldTransform_.translate_ = position;

	deleteTime_ = 180;
	isAlive_ = true;

	//半径
	radius_ = 1.0f;

	//自分
	SetCollisionAttribute(COLLISION_ATTRIBUTE_ENEMY);
	//相手
	SetCollisionMask(COLLISION_ATTRIBUTE_PLAYER);

}

void Enemy::Update(){
	
	//更新
	worldTransform_.Update();


#ifdef _DEBUG
	ImGui::Begin("Enemy");
	ImGui::InputInt("AliveTive", &deleteTime_);
	ImGui::End();
#endif // _DEBUG
}

void Enemy::Draw(Camera& camera){
	
	//描画
	if (isAlive_ == true) {
		model_->Draw(worldTransform_, camera);
	}
	
}

void Enemy::OnCollision(){
	ImGui::Begin("EnemyCollision");
	ImGui::End();


	deleteTime_ -= 1;
	//カウントが0になったら消す
	if (deleteTime_ < 0) {
		isAlive_ = false;
	}
}

Vector3 Enemy::GetWorldPosition(){
	Vector3 result = {};
	result.x = worldTransform_.worldMatrix_.m[3][0];
	result.y = worldTransform_.worldMatrix_.m[3][1];
	result.z = worldTransform_.worldMatrix_.m[3][2];

	return result;
}
