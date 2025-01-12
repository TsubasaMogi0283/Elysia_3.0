#include "StrongEnemy.h"

#include <numbers>

#include "Camera.h"
#include "SpotLight.h"
#include <VectorCalculation.h>
#include "CollisionConfig.h"


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
	//ライティングの種類
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


	//判定
	//自分
	SetCollisionAttribute(COLLISION_ATTRIBUTE_STRONG_ENEMY);
	//相手
	SetCollisionMask(COLLISION_ATTRIBUTE_PLAYER2);

#pragma endregion




	
}


void StrongEnemy::Update(){


	const float SPEED_AMOUNT = 0.02f;
	//状態
	//こっちもStatePatternにしたい！！
	switch (condition_) {
	case EnemyCondition::NoneMove:
		//何もしない

		speed_ = { 0.0f,0.0f,0.0f };
		break;

		
	case EnemyCondition::Move:

		//通常の動き
		//スピードの正規化
		if (speed_.x != 0.0f ||
			speed_.y != 0.0f ||
			speed_.z != 0.0f) {
			direction_ = VectorCalculation::Normalize(speed_);
		}

		//スピードの計算
		Vector3 moveSpeedVelocity = VectorCalculation::Multiply(direction_, SPEED_AMOUNT);
		//加算
		worldTransform_.translate = VectorCalculation::Add(worldTransform_.translate, moveSpeedVelocity);


		break;



	case EnemyCondition::Tracking:

		#pragma region 追跡

		//向きを求める
		direction_ = VectorCalculation::Subtract(playerPosition_, GetWorldPosition());
		//正規化
		direction_ = VectorCalculation::Normalize(direction_);

		//
		direction_ = VectorCalculation::Multiply(direction_, SPEED_AMOUNT);
		//加算
		worldTransform_.translate = VectorCalculation::Add(worldTransform_.translate, direction_);



#ifdef _DEBUG
		ImGui::Begin("Tracking");
		ImGui::InputFloat3("方向", &direction_.x);
		ImGui::InputFloat3("プレイヤーの座標", &playerPosition_.x);

		ImGui::End();
#endif // DEBUG



		#pragma endregion

		break;
	}





	//向きを計算しモデルを回転させる
	float directionToRotateY = std::atan2f(-direction_.z, direction_.x);
	//修正
	const float ROTATE_OFFSET = -std::numbers::pi_v<float> / 2.0f;
	//加算
	worldTransform_.rotate.y = directionToRotateY + ROTATE_OFFSET;

	

	//ワールドトランスフォームの更新
	worldTransform_.Update();

	//マテリアルの更新
	material_.Update();


	//AABBの計算
	aabb_.min = VectorCalculation::Subtract(GetWorldPosition(), { radius_ ,radius_ ,radius_ });
	aabb_.max = VectorCalculation::Add(GetWorldPosition(), { radius_ ,radius_ ,radius_ });


}

void StrongEnemy::Draw(const Camera& camera,const SpotLight& spotLight){
	//描画
	model_->Draw(worldTransform_, camera, material_, spotLight);
}



void StrongEnemy::OnCollision(){
	//プレイヤーと接触した
	isTouchPlayer_ = true;

}

void StrongEnemy::OffCollision(){

}
