#include "StrongEnemyTracking.h"

#include "VectorCalculation.h"
#include "Enemy/StrongEnemy/StrongEnemy.h"

void StrongEnemyTracking::Initialize()
{
}

void StrongEnemyTracking::Update(StrongEnemy* strongEnemy){


	const float SPEED_AMOUNT = 0.02f;
	Vector3 worldPosition = strongEnemy->GetWorldPosition();
	Vector3 playerPosition = strongEnemy->GetPlayerPosition();


	//向きを求める
	Vector3 direction = VectorCalculation::Subtract(playerPosition, worldPosition);
	//正規化
	direction = VectorCalculation::Normalize(direction);

	//
	direction = VectorCalculation::Multiply(direction, SPEED_AMOUNT);
	//加算
	strongEnemy->SetTranslate(VectorCalculation::Add(worldPosition, direction));
	strongEnemy->SetDirection(direction);

#ifdef _DEBUG
	ImGui::Begin("強敵追跡");
	ImGui::InputFloat3("方向", &direction_.x);
	ImGui::InputFloat3("プレイヤーの座標", &playerPosition.x);

	ImGui::End();
#endif // DEBUG


}
