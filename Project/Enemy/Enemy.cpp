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
	radius_ = 0.5f;
	speed_ = speed;
	//色
	color_ = { 1.0f,1.0f,1.0f,1.0f };
	//デフォルトで右方向に向いているようにする
	direction_ = { 1.0f,0.0f,0.0f };

	condition_ = EnemyCondition::Move;

	attackTime_ = 0;
	//自分
	SetCollisionAttribute(COLLISION_ATTRIBUTE_ENEMY);
	//相手
	SetCollisionMask(COLLISION_ATTRIBUTE_PLAYER);
#ifdef _DEBUG
	uint32_t debugModelHandle = ModelManager::GetInstance()->LoadModelFile("Resources/CG3/Sphere", "Sphere.obj");
	debugModel_.reset(Model::Create(debugModelHandle));

	debugModelWorldTransform_.Initialize();
	debugModelWorldTransform_.scale_ = { 0.5f,0.5f,0.5f };
#endif // _DEBUG

	
}


void Enemy::Update(){
	playerPosition_;
	
	switch (condition_) {
		//何も攻撃しない
	case EnemyCondition::NoneMove:
		#ifdef _DEBUG
		ImGui::Begin("None");
		ImGui::End();
		#endif // DEBUG
	
		t_ = 0.0f;
		preTrackingPlayerPosition_ = {};
		preTrackingPosition_ = {};
		speed_ = { 0.0f,0.0f,0.0f };
		break;
	
		//通常の動き
	case EnemyCondition::Move:
		#ifdef _DEBUG
		ImGui::Begin("Move");
		ImGui::End();
		#endif // DEBUG
		//通常の動き
		t_ = 0.0f;
		preTrackingPlayerPosition_ = {};
		preTrackingPosition_ = {};
		if (speed_.x != 0.0f ||
			speed_.y != 0.0f ||
			speed_.z != 0.0f) {
			direction_ = VectorCalculation::Normalize(speed_);
		}
		
		worldTransform_.translate_ = VectorCalculation::Add(worldTransform_.translate_, speed_);
	
	
		break;
	
	//case EnemyCondition::PreTracking:
	//
	//	#pragma region 追跡準備
	//
	//	#ifdef _DEBUG
	//		ImGui::Begin("PreTracking");
	//		ImGui::End();
	//	#endif // DEBUG
	//
	//	//取得したら追跡
	//	preTrackingPlayerPosition_ = playerPosition_;
	//	preTrackingPosition_ = GetWorldPosition();
	//	
	//
	//	#pragma endregion
	//
	//	condition_ = EnemyCondition::Tracking;
	//
	//	break;
	//
	//
	//
	//	//追跡
	//case EnemyCondition::Tracking:
	//
	//	#pragma region 追跡処理
	//
	//	#ifdef _DEBUG
	//		ImGui::Begin("Tracking");
	//		ImGui::End();
	//	#endif // DEBUG
	//	t_ += 0.005f;
	//	worldTransform_.translate_ = VectorCalculation::Lerp(preTrackingPosition_, preTrackingPlayerPosition_, t_);
	//	
	//	//向きを求める
	//	direction_ = VectorCalculation::Subtract(preTrackingPlayerPosition_,preTrackingPosition_);
	//	direction_ = VectorCalculation::Normalize(direction_);
	//
	//	#pragma endregion
	//
	//
	//
	//	break;
	//
	//	//攻撃
	//case EnemyCondition::Attack:
	//	attackTime_ += 1;
	//	
	//	if (attackTime_ > 1 && attackTime_ <= 160) {
	//	#ifdef _DEBUG
	//		ImGui::Begin("Attack");
	//		ImGui::End();
	//	#endif // DEBUG
	//	}
	//
	//	if (attackTime_ > 180) {
	//		attackTime_ = 0;
	//	}
	//
	//
	//
	//
	}

	//direction_ = VectorCalculation::Normalize(speed_);
	//worldTransform_.translate_ = VectorCalculation::Add(worldTransform_.translate_, speed_);


#ifdef _DEBUG
	const float INTERVAL = 3.0f;
	Vector3 lineEnd = VectorCalculation::Add(GetWorldPosition(), { direction_.x * INTERVAL,direction_.y * INTERVAL,direction_.z * INTERVAL });
	debugModelWorldTransform_.translate_ = lineEnd;
	debugModelWorldTransform_.Update();

#endif // _DEBUG

	
	//更新
	worldTransform_.Update();
	material_.Update();


#ifdef _DEBUG
	ImGui::Begin("Enemy");
	ImGui::InputFloat("T", &t_);
	ImGui::InputFloat3("direction_", &direction_.x);
	ImGui::InputFloat3("Position", &worldTransform_.translate_.x);
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



	//0になったら消す
	if (color_.y < 0.0f &&
		color_.z < 0.0f) {
		isAlive_ = false;
	}


	material_.color_ = color_;
	
}

Vector3 Enemy::GetWorldPosition() {
	Vector3 result = {
		.x= worldTransform_.worldMatrix_.m[3][0],
		.y = worldTransform_.worldMatrix_.m[3][1],
		.z = worldTransform_.worldMatrix_.m[3][2],
	};

	return result;
}

void Enemy::SetTranslate(Vector3& translate) {
	this->worldTransform_.translate_.x = translate.x;
	this->worldTransform_.translate_.y = translate.y;
	this->worldTransform_.translate_.z = translate.z;

}




void Enemy::Draw(Camera& camera,SpotLight&spotLight){
#ifdef _DEBUG
	debugModel_->Draw(debugModelWorldTransform_, camera, material_, spotLight);

#endif // _DEBUG

	

	//描画
	if (isAlive_ == true) {
		model_->Draw(worldTransform_, camera,material_, spotLight);
	}
	
}

