#include "NormalEnemyTracking.h"

#include "VectorCalculation.h"
#include "Enemy/NormalEnemy/NormalEnemy.h"

NormalEnemyTracking::NormalEnemyTracking(){
	stateName_ = "Tracking";
}

void NormalEnemyTracking::Initialize(){

}

void NormalEnemyTracking::Update(NormalEnemy* normalEnemy){
	//強敵本体の座標を取得
	Vector3 worldPosition = normalEnemy->GetWorldPosition();
	//プレイヤーの座標を取得
	Vector3 playerPosition = normalEnemy->GetPlayerPosition();

	//向きを求める
	direction_ = VectorCalculation::Subtract(playerPosition, worldPosition);
	//正規化
	direction_ = VectorCalculation::Normalize(direction_);
	//スピード
	const float SPEED_AMOUNT = 0.05f;
	Vector3 speed = VectorCalculation::Multiply(direction_, SPEED_AMOUNT);
	//加算
	normalEnemy->SetTranslate(VectorCalculation::Add(worldPosition, speed));

#ifdef _DEBUG
	ImGui::Begin("強敵追跡");
	ImGui::InputFloat3("方向", &direction_.x);

	ImGui::End();
#endif // DEBUG


}
