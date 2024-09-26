#include "PlayerCollisionToStrongEnemy.h"
#include <Collider/CollisionConfig.h>

void PlayerCollisionToStrongEnemy::Initialize(){

	uint32_t modelHandle= ModelManager::GetInstance()->LoadModelFile("Resources/CG3/Sphere", "Sphere.obj");
	model_.reset(Model::Create(modelHandle));


	//初期化
	worldTransform_.Initialize();
	material_.Initialize();
	material_.lightingKinds_ = Spot;
	material_.color_ = { .x = 1.0f,.y = 1.0f,.z = 1.0f,.w = 1.0f };

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
	worldTransform_.translate_ = playerWorldPosition_;

	material_.Update();
	worldTransform_.Update();


}

void PlayerCollisionToStrongEnemy::Draw(Camera& camera, SpotLight& spotLight){

#ifdef _DEBUG
	model_->Draw(worldTransform_, camera, material_, spotLight);
#endif // _DEBUG

}

Vector3 PlayerCollisionToStrongEnemy::GetWorldPosition(){
	Vector3 position = {
		.x = worldTransform_.worldMatrix_.m[3][0],
		.y = worldTransform_.worldMatrix_.m[3][1],
		.z = worldTransform_.worldMatrix_.m[3][2],

	};
	return position;
}

void PlayerCollisionToStrongEnemy::OnCollision()
{
}
