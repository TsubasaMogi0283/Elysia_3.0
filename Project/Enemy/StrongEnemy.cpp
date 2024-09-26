#include "StrongEnemy.h"

#include <numbers>

#include "Camera.h"
#include "SpotLight.h"
#include <VectorCalculation.h>
#include <Collider/CollisionConfig.h>


void StrongEnemy::Initialize(uint32_t& modelHandle, Vector3& position, Vector3& speed){
	//モデル
	model_.reset(Model::Create(modelHandle));

	//ワールドトランスフォーム
	worldTransform_.Initialize();
	const float SCALE_SIZE = 2.0f;
	worldTransform_.scale_ = { .x = SCALE_SIZE ,.y = SCALE_SIZE ,.z = SCALE_SIZE };
	worldTransform_.translate_ = position;






	//スピード
	speed_ = speed;

	//マテリアル
	material_.Initialize();
	material_.lightingKinds_ = Spot;

	//プレイヤーに当たったかどうか
	isTouchPlayer_ = false;


#pragma region 当たり判定

	collisionType_ = CollisionType::SphereType;
	//半径
	radius_ = 2.0f;

	//AABBのmax部分に加算する縦横高さのサイズ
	upSideSize_ = { .x = radius_ ,.y = radius_ ,.z = radius_ };

	//AABBのmin部分に加算する縦横高さのサイズ
	downSideSize_ = { .x = radius_ ,.y = radius_ ,.z = radius_ };

	//判定
	//自分
	SetCollisionAttribute(COLLISION_ATTRIBUTE_STRONG_ENEMY);
	//相手
	SetCollisionMask(COLLISION_ATTRIBUTE_PLAYER);

#pragma endregion



}

void StrongEnemy::Update(){



	const float SPEED_AMOUNT = 0.05f;
//	//状態
//	switch (condition_) {
//		//何もしない
//	case EnemyCondition::NoneMove:
//#ifdef _DEBUG
//		ImGui::Begin("None");
//		ImGui::End();
//#endif // DEBUG
//
//		preTrackingPlayerPosition_ = {};
//		preTrackingPosition_ = {};
//		speed_ = { 0.0f,0.0f,0.0f };
//		break;
//
//		//通常の動き
//	case EnemyCondition::Move:
//
//
//
//
//		//通常の動き
//		preTrackingPlayerPosition_ = {};
//		preTrackingPosition_ = {};
//		if (speed_.x != 0.0f ||
//			speed_.y != 0.0f ||
//			speed_.z != 0.0f) {
//			direction_ = VectorCalculation::Normalize(speed_);
//		}
//
//		worldTransform_.translate_ = VectorCalculation::Add(worldTransform_.translate_, speed_);
//
//
//		break;
//
//	case EnemyCondition::PreTracking:
//
//#pragma region 追跡準備
//
//
//		//取得したら追跡
//		preTrackingPlayerPosition_ = playerPosition_;
//		preTrackingPosition_ = GetWorldPosition();
//
//
//#pragma endregion
//
//
//		//強制的に追跡
//		preCondition_ = EnemyCondition::PreTracking;
//		condition_ = EnemyCondition::Tracking;
//
//		break;
//
//		//追跡
//	case EnemyCondition::Tracking:
//		//追跡処理
//
//
//#ifdef _DEBUG
//		ImGui::Begin("Tracking");
//		ImGui::End();
//#endif // DEBUG
//
//
//
//		//向きを求める
//		direction_ = VectorCalculation::Subtract(preTrackingPlayerPosition_, preTrackingPosition_);
//		direction_ = VectorCalculation::Normalize(direction_);
//
//		//加算
//		Vector3 speedVelocity = VectorCalculation::Multiply(direction_, SPEED_AMOUNT);
//		worldTransform_.translate_ = VectorCalculation::Add(worldTransform_.translate_, speedVelocity);
//
//
//		break;
//
//
//	}

	//向きを求める
	direction_ = VectorCalculation::Normalize(speed_);



	//向きを計算しモデルを回転させる
	float directionToRotateY = std::atan2f(-direction_.z, direction_.x);
	const float ROTATE_OFFSET = -std::numbers::pi_v<float> / 2.0f;
	worldTransform_.rotate_.y = directionToRotateY + ROTATE_OFFSET;

	

	//座標の計算
	Vector3 newSpeed = VectorCalculation::Multiply(direction_, SPEED_AMOUNT);
	worldTransform_.translate_ = VectorCalculation::Add(worldTransform_.translate_, newSpeed);



	//ワールドトランスフォーム
	worldTransform_.Update();

	//マテリアル
	material_.Update();



	//AABBの計算
	aabb_.min = VectorCalculation::Subtract(GetWorldPosition(), downSideSize_);
	aabb_.max = VectorCalculation::Add(GetWorldPosition(), upSideSize_);



}

void StrongEnemy::Draw(Camera& camera, SpotLight& spotLight){
	model_->Draw(worldTransform_, camera, material_, spotLight);
}

void StrongEnemy::OnCollision(){
	isTouchPlayer_ = true;

}


Vector3 StrongEnemy::GetWorldPosition(){
	Vector3 position = {
		.x = worldTransform_.worldMatrix_.m[3][0],
		.y = worldTransform_.worldMatrix_.m[3][1],
		.z = worldTransform_.worldMatrix_.m[3][2],

	};

	return position;
}
