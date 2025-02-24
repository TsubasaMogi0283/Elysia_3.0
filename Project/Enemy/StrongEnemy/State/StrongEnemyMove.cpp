#include "StrongEnemyMove.h"

#include "Enemy/StrongEnemy/StrongEnemy.h"
#include "VectorCalculation.h"


void StrongEnemyMove::Initialize()
{
}

void StrongEnemyMove::Update(StrongEnemy* strongEnemy){

	const float SPEED_AMOUNT = 0.02f;
	Vector3 worldPosition = strongEnemy->GetWorldPosition();
	Vector3 playerPosition = strongEnemy->GetPlayerPosition();

	//向きを求める
	Vector3 direction = VectorCalculation::Subtract(playerPosition, worldPosition);
	//正規化
	direction = VectorCalculation::Normalize(direction);
	//スピードの値をかける
	direction = VectorCalculation::Multiply(direction, SPEED_AMOUNT);
	//加算
	strongEnemy->SetTranslate(VectorCalculation::Add(worldPosition, direction));
	//方向を設定し直す
	strongEnemy->SetDirection(direction);

#ifdef _DEBUG
	ImGui::Begin("強敵通常動作");
	ImGui::InputFloat3("方向", &direction.x);
	ImGui::InputFloat3("プレイヤーの座標", &playerPosition.x);

	ImGui::End();
#endif // DEBUG




}
