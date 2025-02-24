#include "StrongEnemyNoneMove.h"

#include "Enemy/StrongEnemy/StrongEnemy.h"

void StrongEnemyNoneMove::Initialize(){
	//何もしないので0にする
	speed_ = { .x = 0.0f,.y = 0.0f,.z = 0.0f };
}

void StrongEnemyNoneMove::Update(const std::unique_ptr<StrongEnemy>& strongEnemy){
	
	//スピードの設定
	const Vector3 SPEED = { .x = 0.0f,.y = 0.0f,.z = 0.0f };
	strongEnemy->SetSpeed(speed_);
}
