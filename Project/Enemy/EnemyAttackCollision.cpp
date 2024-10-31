#include "EnemyAttackCollision.h"
#include <Collider/CollisionConfig.h>
#include <VectorCalculation.h>


void EnemyAttackCollision::Initialize(uint32_t modelHandle){
	model_.reset(Model::Create(modelHandle));


	//初期化
	worldTransform_.Initialize();
	material_.Initialize();
	material_.lightingKinds_ = Spot;
	material_.color_ = { .x = 1.0f,.y = 1.0f,.z = 1.0f,.w = 1.0f };

	enemyWorldPosition_ = {};


	#pragma region 当たり判定

	//種類
	collisionType_ = CollisionType::SphereType;

	//半径
	radius_ = 1.0f;



	//自分
	SetCollisionAttribute(COLLISION_ATTRIBUTE_ENEMY_ATTACK);
	//相手
	SetCollisionMask(COLLISION_ATTRIBUTE_PLAYER);

	#pragma endregion

}

void EnemyAttackCollision::Update(){

	
	//間隔
	const float INTERVAL = 5.0f;
	Vector3 newDirection = VectorCalculation::Multiply(enemyDirection_, INTERVAL);
	//更新
	worldTransform_.translate_ = VectorCalculation::Add(enemyWorldPosition_, newDirection);
	worldTransform_.Update();
	material_.Update();
}

void EnemyAttackCollision::Draw(Camera& camera, SpotLight& spotLight){

#ifdef _DEBUG
	if (isTouch_ == true) {
		model_->Draw(worldTransform_, camera, material_, spotLight);
	}
#endif // _DEBUG


	
}



Vector3 EnemyAttackCollision::GetWorldPosition(){
	Vector3 worldPosition = {
		.x = worldTransform_.worldMatrix_.m[3][0],
		.y = worldTransform_.worldMatrix_.m[3][1],
		.z = worldTransform_.worldMatrix_.m[3][2],
	};
	return worldPosition;
}

void EnemyAttackCollision::OnCollision(){

}

void EnemyAttackCollision::OffCollision()
{
}

