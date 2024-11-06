#include "PlayerCollisionToStrongEnemy.h"
#include <Collider/CollisionConfig.h>
#include <ModelManager.h>

void PlayerCollisionToStrongEnemy::Initialize(){

	uint32_t modelHandle= ModelManager::GetInstance()->LoadModelFile("Resources/CG3/Sphere", "Sphere.obj");
	model_.reset(Model::Create(modelHandle));


	//初期化
	worldTransform_.Initialize();
	playerWorldPosition_ = {};


#pragma region 当たり判定

	//種類
	collisionType_ = CollisionType::SphereType;

	//半径
	radius_ = 1.0f;

	

	//自分
	SetCollisionAttribute(COLLISION_ATTRIBUTE_PLAYER2);
	//相手
	SetCollisionMask(COLLISION_ATTRIBUTE_STRONG_ENEMY);

#pragma endregion
}

void PlayerCollisionToStrongEnemy::Update(){
	//プレイヤーの座標を持ってくる
	worldTransform_.translate = playerWorldPosition_;

	worldTransform_.Update();


}

void PlayerCollisionToStrongEnemy::Draw(Camera& camera, Material& material, SpotLight& spotLight){

#ifdef _DEBUG
	model_->Draw(worldTransform_, camera, material, spotLight);
#endif // _DEBUG

}

Vector3 PlayerCollisionToStrongEnemy::GetWorldPosition(){
	Vector3 position = worldTransform_.GetWorldPosition();
	return position;
}

void PlayerCollisionToStrongEnemy::OnCollision(){

}

void PlayerCollisionToStrongEnemy::OffCollision()
{
}
