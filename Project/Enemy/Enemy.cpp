#include "Enemy.h"

#include "../Collider/CollisionConfig.h"
#include <VectorCalculation.h>

#include "SpotLight.h"
#include <SingleCalculation.h>
#include <numbers>

void Enemy::Initialize(uint32_t modelHandle, Vector3 position, Vector3 speed){
	
	model_.reset(Model::Create(modelHandle));
	worldTransform_.Initialize();

	worldTransform_.scale_ = { .x = SCALE_SIZE,.y = SCALE_SIZE ,.z = SCALE_SIZE };
#ifdef _DEBUG
	float DEBUG_SCALE = 1.0f;
	worldTransform_.scale_ = { .x = DEBUG_SCALE,.y = DEBUG_SCALE ,.z = DEBUG_SCALE };
#endif // _DEBUG

	//座標の代入
	worldTransform_.translate_ = position;

	//マテリアルの初期化
	material_.Initialize();
	material_.lightingKinds_ = Spot;

	//生存か死亡
	isAlive_ = true;
	deleteTime_ = 180;
	
	//追跡かどうか
	isTracking_ = false;

	//半径
	radius_ = 1.0f;
	preSpeed_ = speed;
	speed_ = speed;

	//色
	color_ = { 1.0f,1.0f,1.0f,1.0f };
	//デフォルトで右方向に向いているようにする
	direction_ = { 1.0f,0.0f,0.0f };

	//状態
	preCondition_ = EnemyCondition::NoneMove;
	condition_ = EnemyCondition::Move;

	//攻撃
	attackTime_ = 0;

	//判定
	//自分
	SetCollisionAttribute(COLLISION_ATTRIBUTE_ENEMY);
	//相手
	SetCollisionMask(COLLISION_ATTRIBUTE_PLAYER);
#ifdef _DEBUG
	uint32_t debugModelHandle = ModelManager::GetInstance()->LoadModelFile("Resources/CG3/Sphere", "Sphere.obj");
	debugModel_.reset(Model::Create(debugModelHandle));

	debugModelWorldTransform_.Initialize();
	const float DEBUG_MODEL_SCALE = 0.25f;
	debugModelWorldTransform_.scale_ = { .x= DEBUG_MODEL_SCALE,.y= DEBUG_MODEL_SCALE,.z= DEBUG_MODEL_SCALE };
#endif // _DEBUG

	
}


void Enemy::Update(){
	

//#pragma region ステージの外に行かないようにする
//
//	Vector3 position = enemy->GetWorldPosition();
//	float xxxx = stageRect_.leftBack.x + enemy->GetRadius();
//	//左
//	if (position.x < xxxx) {
//#ifdef _DEBUG
//		ImGui::Begin("Aaaaa");
//		ImGui::End();
//#endif // _DEBUG
//		enemy->InvertSpeedX();
//
//		//enemy->InvertSpeedX();
//	}
//
//#pragma endregion
	//更新
	worldTransform_.Update();
	material_.Update();


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


		if (GetWorldPosition().x < stageRect_.leftBack.x + radius_) {
#ifdef _DEBUG
			ImGui::Begin("AAAAAAAAA"); 
			ImGui::End();
#endif // _DEBUG

			speed_.x *= -1.0f;


		}

		//if (preCondition_ == EnemyCondition::NoneMove) {
		//	speed_ = preSpeed_;
		//}
		//else if (preCondition_ == EnemyCondition::NoneMove) {
		//	speed_ = preSpeed_;
		//}
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
	
	case EnemyCondition::PreTracking:
	
		#pragma region 追跡準備
	
		#ifdef _DEBUG
			ImGui::Begin("PreTracking");
			ImGui::End();
		#endif // DEBUG
	
		//取得したら追跡
		preTrackingPlayerPosition_ = playerPosition_;
		preTrackingPosition_ = GetWorldPosition();
		
	
		#pragma endregion
	
		condition_ = EnemyCondition::Tracking;
	
		break;
	
	
	
		//追跡
	case EnemyCondition::Tracking:
	
		#pragma region 追跡処理
	
		#ifdef _DEBUG
			ImGui::Begin("Tracking");
			ImGui::End();
		#endif // DEBUG
		t_ += 0.005f;
		worldTransform_.translate_ = VectorCalculation::Lerp(preTrackingPosition_, preTrackingPlayerPosition_, t_);
		
		//向きを求める
		direction_ = VectorCalculation::Subtract(preTrackingPlayerPosition_,preTrackingPosition_);
		direction_ = VectorCalculation::Normalize(direction_);
	
		#pragma endregion

		break;
	
		//攻撃
	case EnemyCondition::Attack:
		attackTime_ += 1;
		
		if (attackTime_ > 60 && attackTime_ <= 180) {
		#ifdef _DEBUG
			ImGui::Begin("Attack");
			ImGui::End();
		#endif // DEBUG
		}
	
		//4秒経ったらまた0になる
		if (attackTime_ > 240) {
			attackTime_ = 0;
		}
	
	}
	//向きを計算しモデルを回転させる
	float directionToRotateY = std::atan2f(direction_.z,direction_.x);
	const float ROTATE_OFFSET = std::numbers::pi_v<float>;
	worldTransform_.rotate_.y = directionToRotateY+ ROTATE_OFFSET;
	

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
	float degreeRotateY = directionToRotateY * (180.0f / std::numbers::pi_v<float>);

	ImGui::Begin("Enemy");
	ImGui::InputFloat("T", &t_);
	ImGui::InputFloat3("direction_", &direction_.x);
	ImGui::InputFloat("RotateY", &degreeRotateY);
	ImGui::InputFloat3("Speed", &direction_.x);

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

