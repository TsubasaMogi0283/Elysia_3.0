#include "StrongEnemyNoneMove.h"

#include "Enemy/StrongEnemy/StrongEnemy.h"

void StrongEnemyNoneMove::Initialize(){

}

void StrongEnemyNoneMove::Update(StrongEnemy* strongEnemy){
	
	//スピードの設定
	const Vector3 SPEED = { .x = 0.0f,.y = 0.0f,.z = 0.0f };
	strongEnemy->SetSpeed(speed_);
}
