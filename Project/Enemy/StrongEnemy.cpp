#include "StrongEnemy.h"

#include <numbers>

#include "Camera.h"
#include "SpotLight.h"
#include <VectorCalculation.h>
#include <Collider/CollisionConfig.h>


void StrongEnemy::Initialize(const uint32_t& modelHandle,const Vector3& position,const Vector3& speed){

	//モデル
	model_.reset(Model::Create(modelHandle));

	//ワールドトランスフォーム
	worldTransform_.Initialize();
	const float SCALE_SIZE = 2.0f;
	worldTransform_.scale = { .x = SCALE_SIZE ,.y = SCALE_SIZE ,.z = SCALE_SIZE };
	worldTransform_.translate = position;


	//スピード
	speed_ = speed;

	//マテリアル
	material_.Initialize();
	material_.lightingKinds_ = Spot;

	//プレイヤーに当たったかどうか
	isTouchPlayer_ = false;



	//状態の初期化
	condition_ = EnemyCondition::Tracking;

#pragma region 当たり判定

	//当たり判定で使う種類
	collisionType_ = ColliderType::SphereType;
	//半径
	radius_ = 2.0f;

	//AABBのmax部分に加算する縦横高さのサイズ
	//upSideSize_ = { .x = radius_ ,.y = radius_ ,.z = radius_ };

	//AABBのmin部分に加算する縦横高さのサイズ
	//downSideSize_ = { .x = radius_ ,.y = radius_ ,.z = radius_ };

	//判定
	//自分
	SetCollisionAttribute(COLLISION_ATTRIBUTE_STRONG_ENEMY);
	//相手
	SetCollisionMask(COLLISION_ATTRIBUTE_PLAYER2);

#pragma endregion




	
}




void StrongEnemy::TrackingAudio(){
	

}


void StrongEnemy::Update(){


	const float SPEED_AMOUNT = 0.05f;
	//状態
	//こっちもStatePatternにしたい！！
	switch (condition_) {
	case EnemyCondition::NoneMove:
		//何もしない
#ifdef _DEBUG
		ImGui::Begin("None");
		ImGui::End();
#endif // DEBUG


		speed_ = { 0.0f,0.0f,0.0f };
		break;

		//通常の動き
	case EnemyCondition::Move:


		//スピードの正規化
		if (speed_.x != 0.0f ||
			speed_.y != 0.0f ||
			speed_.z != 0.0f) {
			direction_ = VectorCalculation::Normalize(speed_);
		}

		//加算
		Vector3 moveSpeedVelocity = VectorCalculation::Multiply(direction_, SPEED_AMOUNT);
		worldTransform_.translate = VectorCalculation::Add(worldTransform_.translate, moveSpeedVelocity);


		break;



	case EnemyCondition::Tracking:

		#pragma region 追跡

		//向きを求める
		direction_ = VectorCalculation::Subtract(playerPosition_, GetWorldPosition());
		direction_ = VectorCalculation::Normalize(direction_);

		//加算
		Vector3 newSpeed = VectorCalculation::Multiply(direction_, SPEED_AMOUNT);
		worldTransform_.translate = VectorCalculation::Add(worldTransform_.translate, newSpeed);



#ifdef _DEBUG
		ImGui::Begin("Tracking");
		ImGui::InputFloat3("方向", &direction_.x);
		ImGui::InputFloat3("プレイヤーの座標", &playerPosition_.x);

		ImGui::End();
#endif // DEBUG



		#pragma endregion

		break;
	}



	//追跡時のオーディオ処理
	TrackingAudio();

	




	//向きを計算しモデルを回転させる
	float directionToRotateY = std::atan2f(-direction_.z, direction_.x);
	const float ROTATE_OFFSET = -std::numbers::pi_v<float> / 2.0f;
	worldTransform_.rotate.y = directionToRotateY + ROTATE_OFFSET;

	

	//ワールドトランスフォームの更新
	worldTransform_.Update();

	//マテリアルの更新
	material_.Update();


	//AABBの計算
	//aabb_.min = VectorCalculation::Subtract(GetWorldPosition(), downSideSize_);
	//aabb_.max = VectorCalculation::Add(GetWorldPosition(), upSideSize_);



}

void StrongEnemy::Draw(const Camera& camera,const SpotLight& spotLight){
	model_->Draw(worldTransform_, camera, material_, spotLight);
}



void StrongEnemy::OnCollision(){
	//プレイヤーと接触した
	isTouchPlayer_ = true;

}

void StrongEnemy::OffCollision(){

}
