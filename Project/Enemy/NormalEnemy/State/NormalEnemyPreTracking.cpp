#include "NormalEnemyPreTracking.h"

#include "VectorCalculation.h"
#include "Enemy/NormalEnemy/NormalEnemy.h"
#include "NormalEnemyTracking.h"

NormalEnemyPreTracking::NormalEnemyPreTracking(){
	stateName_ = "PreTracking";
}

void NormalEnemyPreTracking::Update(NormalEnemy* normalEnemy) {

	////強敵本体の座標を取得
	//Vector3 worldPosition = normalEnemy->GetWorldPosition();
	////プレイヤーの座標を取得
	//Vector3 playerPosition = normalEnemy->GetPlayerPosition();
	//
	//
	////向きを求める
	//direction_ = VectorCalculation::Subtract(playerPosition, worldPosition);
	////正規化
	//direction_ = VectorCalculation::Normalize(direction_);
	//
	////スピードの値をかける
	//const float SPEED_AMOUNT = 0.05f;
	//Vector3 speed = VectorCalculation::Multiply(direction_, SPEED_AMOUNT);
	//加算
	//normalEnemy->SetTranslate(VectorCalculation::Add(worldPosition, speed));
	//追跡へ移動
	normalEnemy->ChengeState(std::make_unique<NormalEnemyTracking>());



}
