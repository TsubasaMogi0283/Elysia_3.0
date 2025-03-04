#include "NormalEnemyPreTracking.h"

#include "VectorCalculation.h"
#include "Enemy/NormalEnemy/NormalEnemy.h"
#include "NormalEnemyTracking.h"

NormalEnemyPreTracking::NormalEnemyPreTracking(){
	stateName_ = "PreTracking";
}

void NormalEnemyPreTracking::Initialize(){

}

void NormalEnemyPreTracking::Update(NormalEnemy* normalEnemy) {
	//追跡へ移動
	normalEnemy->ChengeState(std::make_unique<NormalEnemyTracking>());
}
