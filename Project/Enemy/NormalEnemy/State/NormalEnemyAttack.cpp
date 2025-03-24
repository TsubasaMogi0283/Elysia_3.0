#include "NormalEnemyAttack.h"

#include <imgui.h>

#include "VectorCalculation.h"
#include "Enemy/NormalEnemy/NormalEnemy.h"

NormalEnemyAttack::NormalEnemyAttack(){
	stateName_ = "Attack";
}

void NormalEnemyAttack::Initialize(){

}

void NormalEnemyAttack::Update(NormalEnemy* normalEnemy){
	//強敵本体の座標を取得
	Vector3 worldPosition = normalEnemy->GetWorldPosition();
	//プレイヤーの座標を取得
	Vector3 playerPosition = normalEnemy->GetPlayerPosition();

	//向きを求める
	direction_ = VectorCalculation::Subtract(playerPosition, worldPosition);
	//正規化
	direction_ = VectorCalculation::Normalize(direction_);

	
	//時間が増えていく
	attackTime_ += TIME_INCREASE_VALUE;

	//1秒の時に攻撃
	if (attackTime_ == JUST_ATTACK_TIME) {
		//ここで攻撃
		//コライダーが当たっている時だけ通す
		normalEnemy->SetIsAttack(true);

	}
	else {
		//攻撃しない
		normalEnemy->SetIsAttack(false);
	}

	//また最初に戻る
	if (attackTime_ > RETURN_TIME) {
		attackTime_ = RESTART_TIME;
	}



#ifdef _DEBUG
	int32_t attackTimeToInt = static_cast<int32_t>(attackTime_);
	ImGui::Begin("通常の敵の攻撃");
	ImGui::InputInt("攻撃時間", &attackTimeToInt);
	ImGui::End();
#endif // _DEBUG


}

NormalEnemyAttack::~NormalEnemyAttack(){
	//0で設定し攻撃していないようにする
	attackTime_ = 0u;
}
