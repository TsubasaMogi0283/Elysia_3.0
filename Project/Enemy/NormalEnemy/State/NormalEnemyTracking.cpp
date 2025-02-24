#include "NormalEnemyTracking.h"

#include "VectorCalculation.h"
#include "Enemy/NormalEnemy/NormalEnemy.h"

void NormalEnemyTracking::Initialize(){
	stateName_ = "Tracking";
}

void NormalEnemyTracking::Update(NormalEnemy* normalEnemy){

	//スピード
	const float SPEED_AMOUNT = 0.05f;
	//強敵本体の座標を取得
	Vector3 worldPosition = normalEnemy->GetWorldPosition();
	//プレイヤーの座標を取得
	Vector3 playerPosition = normalEnemy->GetPlayerPosition();


	//向きを求める
	direction_ = VectorCalculation::Subtract(playerPosition, worldPosition);
	//正規化
	direction_ = VectorCalculation::Normalize(direction_);
	//スピードをかける
	direction_ = VectorCalculation::Multiply(direction_, SPEED_AMOUNT);
	//加算
	normalEnemy->SetTranslate(VectorCalculation::Add(worldPosition, direction_));
	//方向を設定
	normalEnemy->SetDirection(direction_);

#ifdef _DEBUG
	ImGui::Begin("強敵追跡");
	ImGui::InputFloat3("方向", &direction_.x);
	ImGui::InputFloat3("プレイヤーの座標", &playerPosition.x);

	ImGui::End();
#endif // DEBUG


}
