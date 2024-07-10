#include "Enemy.h"

#include "../Collider/CollisionConfig.h"
#include <VectorCalculation.h>

#include "SpotLight.h"

void Enemy::Initialize(uint32_t modelHandle, Vector3 position, Vector3 speed){
	
	model_.reset(Model::Create(modelHandle));
	worldTransform_.Initialize();
	worldTransform_.translate_ = position;

	material_.Initialize();
	material_.lightingKinds_ = Spot;
	deleteTime_ = 180;
	isAlive_ = true;

	//半径
	radius_ = 1.0f;
	speed_ = speed;
	//色
	color_ = { 1.0f,1.0f,1.0f,1.0f };

	//自分
	SetCollisionAttribute(COLLISION_ATTRIBUTE_ENEMY);
	//相手
	SetCollisionMask(COLLISION_ATTRIBUTE_PLAYER);

}

void Enemy::Update(){
	
	//更新
	worldTransform_.translate_ = VectorCalculation::Add(worldTransform_.translate_, speed_);
	worldTransform_.Update();

	material_.Update();
#ifdef _DEBUG
	ImGui::Begin("Enemy");
	ImGui::InputInt("AliveTive", &deleteTime_);
	ImGui::End();
#endif // _DEBUG
}



void Enemy::OnCollision() {
#ifdef _DEBUG
	ImGui::Begin("EnemyCollision");
	ImGui::InputFloat4("Color", &color_.x);
	ImGui::End();

#endif // _DEBUG
	const float COLOR_CHANGE_INTERVAL = 0.005f;
	color_.y -= COLOR_CHANGE_INTERVAL;
	color_.z -= COLOR_CHANGE_INTERVAL;



	//カウントが0になったら消す
	if (color_.y < 0.0f &&
		color_.z < 0.0f) {
		isAlive_ = false;
	}


	material_.color_ = color_;
	
}

Vector3 Enemy::GetWorldPosition() {
	Vector3 result = {};
	result.x = worldTransform_.worldMatrix_.m[3][0];
	result.y = worldTransform_.worldMatrix_.m[3][1];
	result.z = worldTransform_.worldMatrix_.m[3][2];

	return result;
}

void Enemy::SetTranslate(Vector3 translate) {
	this->worldTransform_.translate_.x = translate.x;
	this->worldTransform_.translate_.y = translate.y;
	this->worldTransform_.translate_.z = translate.z;

}

void Enemy::SetSammeEnemyCollisionAfter(Vector3 position, Vector3 speed) {
	position;
	speed;
}




void Enemy::Draw(Camera& camera,SpotLight&spotLight){
	
	//描画
	if (isAlive_ == true) {
		model_->Draw(worldTransform_, camera,material_, spotLight);
	}
	
}

