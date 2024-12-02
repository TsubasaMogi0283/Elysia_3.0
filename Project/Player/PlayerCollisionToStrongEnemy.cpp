#include "PlayerCollisionToStrongEnemy.h"
#include <Collider/CollisionConfig.h>
#include <ModelManager.h>

void PlayerCollisionToStrongEnemy::Initialize(){

	//モデルの生成
	uint32_t modelHandle= ModelManager::GetInstance()->LoadModelFile("Resources/Sample/Sphere", "Sphere.obj");
	model_.reset(Model::Create(modelHandle));


	//初期化
	worldTransform_.Initialize();


#pragma region 当たり判定

	//球じゃなくてAABBの方が良いかもね
	//計算の量が減るからね

	//種類
	collisionType_ = ColliderType::SphereType;

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
	//更新
	worldTransform_.Update();


}

void PlayerCollisionToStrongEnemy::Draw(const Camera& camera,const Material& material,const SpotLight& spotLight){

#ifdef _DEBUG
	model_->Draw(worldTransform_, camera, material, spotLight);
#endif // _DEBUG

}


void PlayerCollisionToStrongEnemy::OnCollision(){
	
}

void PlayerCollisionToStrongEnemy::OffCollision()
{
}
