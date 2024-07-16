#include "Enemy.h"

#include "../Collider/CollisionConfig.h"
#include <VectorCalculation.h>

#include "SpotLight.h"
#include <SingleCalculation.h>

void Enemy::Initialize(uint32_t modelHandle, Vector3 position, Vector3 speed){
	
	model_.reset(Model::Create(modelHandle));
	worldTransform_.Initialize();
	worldTransform_.translate_ = position;

	material_.Initialize();
	material_.lightingKinds_ = Spot;
	deleteTime_ = 180;
	isAlive_ = true;
	isTracking_ = false;

	//半径
	radius_ = 1.0f;
	speed_ = speed;
	//色
	color_ = { 1.0f,1.0f,1.0f,1.0f };

	condition = EnemyCondition::Move;

	attackTime_ = 0;
	//自分
	SetCollisionAttribute(COLLISION_ATTRIBUTE_ENEMY);
	//相手
	SetCollisionMask(COLLISION_ATTRIBUTE_PLAYER);

}

void Enemy::Update(){
	//プレイヤーとの距離を求める
	Vector3 difference = VectorCalculation::Subtract(playerPosition_, GetWorldPosition());
	float distance = sqrtf(std::powf(difference.x, 2.0f) + std::powf(difference.y, 2.0f) + std::powf(difference.z, 2.0f));
	const float ATTACK_DISTANCE_OFFSET = 0.0f;
	float MINIMUM_DISTANCE = playerRadius_ + radius_+ ATTACK_DISTANCE_OFFSET;

	//接近するときの距離
	const float TRACKING_START_DISTANCE_ = 10.0f;


	//攻撃するときの距離
	const float ATTACK_START_DISTANCE_ = 6.0f;


	if (distance > TRACKING_START_DISTANCE_) {
		condition = EnemyCondition::Move;
	}

	
	switch (condition) {
	case EnemyCondition::None:
#ifdef _DEBUG
		ImGui::Begin("None");
		ImGui::End();
#endif // DEBUG
		break;

	case EnemyCondition::Move:
#ifdef _DEBUG
		ImGui::Begin("Move");
		ImGui::End();
#endif // DEBUG
		//通常の動き
		t_ = 0.0f;
		preTrackingPlayerPosition_ = {};
		preTrackingPosition_ = {};
		direction_ = VectorCalculation::Normalize(speed_);
		worldTransform_.translate_ = VectorCalculation::Add(worldTransform_.translate_, speed_);


		//設定した値より短くなったら接近開始
		if (distance <= TRACKING_START_DISTANCE_ ) {

			condition = EnemyCondition::PreTracking;
		}

		break;

	case EnemyCondition::PreTracking:
#ifdef _DEBUG
		ImGui::Begin("PreTracking");
		ImGui::End();
#endif // DEBUG
		//取得したら追跡
		preTrackingPlayerPosition_ = playerPosition_;
		preTrackingPosition_ = GetWorldPosition();
		condition = EnemyCondition::Tracking;
		break;

	case EnemyCondition::Tracking:
#ifdef _DEBUG
		ImGui::Begin("Tracking");
		ImGui::End();
#endif // DEBUG
		t_ += 0.005f;
		worldTransform_.translate_ = VectorCalculation::Lerp(preTrackingPosition_, preTrackingPlayerPosition_, t_);
		
		//向きを求める
		direction_ = VectorCalculation::Subtract(preTrackingPlayerPosition_,preTrackingPosition_);
		direction_ = VectorCalculation::Normalize(direction_);


		//Moveへ
		if (distance > TRACKING_START_DISTANCE_) {
			condition = EnemyCondition::Move;
		}
		 
		//設定した値より短くなったら攻撃開始
		if (distance <= ATTACK_START_DISTANCE_ &&
			MINIMUM_DISTANCE < distance) {

			condition = EnemyCondition::Attck;
		}

		break;

	case EnemyCondition::Attck:
		attackTime_ += 1;
		
		if (attackTime_ > 1 && attackTime_ <= 160) {
#ifdef _DEBUG
			ImGui::Begin("Attack");
			ImGui::End();
#endif // DEBUG
		}

		if (attackTime_ > 180) {
			attackTime_ = 0;
		}


		//攻撃し終わった後通常の動きに戻る
		if (attackTime_ > 180 &&
			(distance>=ATTACK_START_DISTANCE_&&
				distance<TRACKING_START_DISTANCE_)) {
			attackTime_ = 0;
			condition = EnemyCondition::Move;
		}



	}


	//更新
	worldTransform_.Update();

	material_.Update();
#ifdef _DEBUG
	ImGui::Begin("Enemy");
	ImGui::InputFloat("T", &t_);
	ImGui::InputFloat3("direction_", &direction_.x);
	ImGui::InputFloat3("Position", &worldTransform_.translate_.x);
	ImGui::InputFloat("distance", &distance);
	ImGui::InputFloat("MINIMUM_DISTANCE", &MINIMUM_DISTANCE);
	ImGui::InputFloat3("preTrackingPlayerPosition", &preTrackingPlayerPosition_.x);
	ImGui::InputFloat3("preTrackingPosition_", &preTrackingPosition_.x);
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




void Enemy::Draw(Camera& camera,SpotLight&spotLight){
	
	//描画
	if (isAlive_ == true) {
		model_->Draw(worldTransform_, camera,material_, spotLight);
	}
	
}

