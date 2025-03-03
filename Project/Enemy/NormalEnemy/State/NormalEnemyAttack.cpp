#include "NormalEnemyAttack.h"

#include "VectorCalculation.h"
#include "Enemy/NormalEnemy/NormalEnemy.h"

NormalEnemyAttack::NormalEnemyAttack(){
	stateName_ = "Attack";
}

void NormalEnemyAttack::Update(NormalEnemy* normalEnemy){
	normalEnemy;
	//0から始める
	const uint32_t RESTART_TIME = 0u;
	//増える値
	const uint32_t TIME_INCREASE_VALUE = 1;
	//時間が増えていく
	attackTime_ += TIME_INCREASE_VALUE;

	//1秒の時に攻撃
	const uint32_t JUST_ATTACK_TIME = 60;
	if (attackTime_ == JUST_ATTACK_TIME) {
		//ここで攻撃
		//コライダーが当たっている時だけ通す
		isAttack_ = true;

	}
	else {
		//攻撃しない
		isAttack_ = false;
	}

	//また最初に戻る
	if (attackTime_ > JUST_ATTACK_TIME * 3) {
		attackTime_ = RESTART_TIME;
	}


}
