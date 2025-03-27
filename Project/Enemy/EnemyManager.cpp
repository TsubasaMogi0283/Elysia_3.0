#include "EnemyManager.h"

#include <imgui.h>
#include <random>
#include <cassert>

#include "Player/Player.h"
#include "VectorCalculation.h"
#include "SingleCalculation.h"
#include "CollisionCalculation.h"
#include "Input.h"
#include "Audio.h"
#include "LevelDataManager.h"


#include "StrongEnemy/State/StrongEnemyNoneMove.h"
#include "StrongEnemy/State/StrongEnemyMove.h"
#include "StrongEnemy/State/StrongEnemyTracking.h"

#include "NormalEnemy/State/NormalEnemyNoneMove.h"
#include "NormalEnemy/State/NormalEnemyMove.h"
#include "NormalEnemy/State/NormalEnemyPreTracking.h"
#include "NormalEnemy/State/NormalEnemyAttack.h"

EnemyManager::EnemyManager(){
	//インスタンスの取得
	//レベルデータ管理クラス
	levelDataManager_ = Elysia::LevelDataManager::GetInstance();
	//オーディオ
	audio_ = Elysia::Audio::GetInstance();
}


void EnemyManager::Initialize(const uint32_t& normalEnemyModel, const uint32_t& strongEnemyModel, const std::vector<Vector3>& normalEnemyPositions, const std::vector<Vector3>& strongEnemyPositions){
	
	//空だったら引っかかるようにしている
	assert(player_!=nullptr);
	assert(levelDataManager_ != nullptr);

	//モデルハンドルを代入
	//通常
	normalEnemyModelHandle_ = normalEnemyModel;
	//強敵
	strongEnemyModelHandle_ = strongEnemyModel;

	//通常の敵の生成
	for (size_t i = 0u; i < normalEnemyPositions.size(); ++i) {
		//生成
		GenerateNormalEnemy(normalEnemyPositions[i]);
	}

	strongEnemyPositions;

	//接近BGMの設定
	audioHandle_ = audio_->Load("Resources/Audio/Enemy/TrackingToPlayer.mp3");
}

void EnemyManager::Update(){

	//接近するときの距離
	const float TRACKING_START_DISTANCE = 15.0f;
	//攻撃するときの距離
	const float ATTACK_START_DISTANCE = 6.0f;
	//プレイヤーの座標
	Vector3 playerPosition = player_->GetWorldPosition();
	//通常の敵
	for (const std::unique_ptr <NormalEnemy>& enemy : enemies_) {
		
		//現在の状態
		std::string currentState = enemy->GetCurrentStateName();

		//プレイヤーの位置を設定
		enemy->SetPlayerPosition(playerPosition);
		//更新
		enemy->Update();


		//移動中の時
		if (currentState == "Move") {

			//レベルエディタから持ってくる
			//AABB
			std::vector<AABB> aabbs = levelDataManager_->GetObjectAABBs(levelDataHandle_, "Stage");
			//コライダーを持っているかどうか
			std::vector<bool> colliders = levelDataManager_->GetIsHavingColliders(levelDataHandle_, "Stage");

			//衝突めり込み防止処理
			for (size_t i = 0u; i < aabbs.size(); ++i) {

				//オブジェクトのAABB
				AABB objectAABB = aabbs[i];
				// AABBを取得
				AABB enemyAABB = enemy->GetAABB();

				//お互いのAABBが接触している場合
				if (CollisionCalculation::IsCollisionAABBPair(enemyAABB, objectAABB) &&
					colliders[i] == true) {

					// 敵とオブジェクトの中心座標を計算
					Vector3 objectCenter = VectorCalculation::Add(objectAABB.min, objectAABB.max);
					objectCenter.x += 0.5f;
					objectCenter.y += 0.5f;
					objectCenter.z += 0.5f;

					// X軸とZ軸の距離を計算
					float distanceX = std::min<float>(abs(enemyAABB.max.x - objectAABB.min.x), abs(enemyAABB.min.x - objectAABB.max.x));
					float distanceZ = std::min<float>(abs(enemyAABB.max.z - objectAABB.min.z), abs(enemyAABB.min.z - objectAABB.max.z));

					//前の座標
					Vector3 enemyPrePosition = enemy->GetWorldPosition();

					//押し戻し分
					const float PUSH_BACK_DISTANCE = 0.1f;

					// 衝突した時の反転処理
					if (distanceX < distanceZ) {
						//X軸の距離が短いとX軸の反転
						enemy->GetCurrentState()->InverseDirectionX();

						//X軸の押し戻し処理
						//押し戻した後に新しく座標を設定
						if (enemyPrePosition.x < objectCenter.x) {
							enemy->SetTranslate({ enemyAABB.min.x - PUSH_BACK_DISTANCE, enemyPrePosition.y, enemyPrePosition.z });
						}
						else {
							enemy->SetTranslate({ enemyAABB.max.x + PUSH_BACK_DISTANCE, enemyPrePosition.y, enemyPrePosition.z });
						}
					}
					else {
						// Z軸の距離が短いとZ軸の反転
						enemy->GetCurrentState()->InverseDirectionZ();

						//Z軸の押し戻し処理
						//押し戻した後に新しく座標を設定
						if (enemyPrePosition.z < objectCenter.z) {
							enemy->SetTranslate({ enemyPrePosition.x, enemyPrePosition.y, enemyAABB.min.z - PUSH_BACK_DISTANCE });
						}
						else {
							enemy->SetTranslate({ enemyPrePosition.x, enemyPrePosition.y, enemyAABB.max.z + PUSH_BACK_DISTANCE });
						}
					}




				}
			}
		}

	}


	//現在の敵の数
	size_t enemyAmount = enemies_.size();
	//1体だけの時
	const uint32_t ONLY_ONE = 1u;
	//敵同士の判定をやる必要が無いからね
	if (enemyAmount == ONLY_ONE) {
		for (const std::unique_ptr <NormalEnemy>& enemy : enemies_) {
			//状態
			std::string currentState = enemy->GetCurrentStateName();
			//向き
			Vector3 enemyDirection = enemy->GetDirection();

			//プレイヤーと敵の差分ベクトル
			Vector3 defference = VectorCalculation::Subtract(playerPosition, enemy->GetWorldPosition());
			//距離
			float defferenceDistance = SingleCalculation::Length(defference);
			

			//通常の動き
			if (currentState == "Move") {
				//近くなったら追跡準備
				if (defferenceDistance < TRACKING_START_DISTANCE ) {
					//追跡準備へ
					enemy->ChengeState(std::make_unique<NormalEnemyPreTracking>());
				}
			}
			//追跡
			else if (currentState == "Tracking") {
				if (defferenceDistance <= ATTACK_START_DISTANCE) {
					//攻撃
					enemy->ChengeState(std::make_unique<NormalEnemyAttack>());
				}
				//距離が離れたら通常の動きへ
				if (defferenceDistance >= TRACKING_START_DISTANCE) {
					//追跡準備へ
					enemy->ChengeState(std::make_unique<NormalEnemyMove>());
				}

			}
			//攻撃
			else if (currentState == "Attack") {
				//攻撃中にプレイヤーが離れた時
				if (defferenceDistance > ATTACK_START_DISTANCE) {
					//通常の動き
					enemy->ChengeState(std::make_unique<NormalEnemyMove>());
				}
			}


		}
	}

	//1体より多い時
	if (enemyAmount > ONLY_ONE) {
		for (std::list<std::unique_ptr<NormalEnemy>>::iterator it1 = enemies_.begin(); it1 != enemies_.end(); ++it1) {

			//比較する数
			const uint32_t COMPARE_NUMBER = 2u;
			//元となる敵
			const uint32_t BASE_ENEMY = 0u;
			//比較する敵
			const uint32_t COMPARE_ENEMY = 1u;

			//AABB
			AABB aabb[COMPARE_NUMBER] = {};
			//座標
			Vector3 enemyPosition[COMPARE_NUMBER] = {};
			
			//元となる敵のAABBと座標、向きを取得
			aabb[BASE_ENEMY] = (*it1)->GetAABB();
			enemyPosition[BASE_ENEMY] = (*it1)->GetWorldPosition();
			Vector3 direction = (*it1)->GetDirection();

			//敵同士の内積
			float enemyAndEnemyDot = 0.0f;
			for (std::list<std::unique_ptr<NormalEnemy>>::iterator it2 = enemies_.begin(); it2 != enemies_.end(); ++it2) {

				//it1とit2が一致した場合は計算をせずに次のループへ
				if (it1 == it2) {
					continue;
				}

				//2体目のAABBを取得
				aabb[COMPARE_ENEMY] = (*it2)->GetAABB();

				//接触している場合
				if ((aabb[BASE_ENEMY].min.x < aabb[COMPARE_ENEMY].max.x && aabb[BASE_ENEMY].max.x > aabb[COMPARE_ENEMY].min.x) &&
					(aabb[BASE_ENEMY].min.y < aabb[COMPARE_ENEMY].max.y && aabb[BASE_ENEMY].max.y > aabb[COMPARE_ENEMY].min.y) &&
					(aabb[BASE_ENEMY].min.z < aabb[COMPARE_ENEMY].max.z && aabb[BASE_ENEMY].max.z > aabb[COMPARE_ENEMY].min.z)) {
					//ワールド座標
					enemyPosition[COMPARE_ENEMY] = (*it2)->GetWorldPosition();


					//敵同士の差分ベクトル
					Vector3 enemyAndEnemyDifference = VectorCalculation::Subtract(enemyPosition[COMPARE_ENEMY], enemyPosition[BASE_ENEMY]);

					//正規化
					Vector3 normalizedEnemyAndEnemy = VectorCalculation::Normalize(enemyAndEnemyDifference);
					//内積
					enemyAndEnemyDot = SingleCalculation::Dot(direction, normalizedEnemyAndEnemy);

					//次のループで上書きされないようにbreakさせるよ！
					break;
				}
			}

			//現在の状態
			std::string currentState = (*it1)->GetCurrentStateName();
			//プレイヤーとの差分
			Vector3 playerEnemyDifference = VectorCalculation::Subtract(playerPosition, (*it1)->GetWorldPosition());
			//プレイヤーとの距離 
			float playerEnemyDistance = SingleCalculation::Length(playerEnemyDifference);

			//通常の動き
			if (currentState == "Move") {

				//前方にいたら行動
				if (playerEnemyDistance < TRACKING_START_DISTANCE) {
					//追跡準備へ
					(*it1)->ChengeState(std::make_unique<NormalEnemyPreTracking>());
				}
			}
			//追跡
			else if (currentState == "Tracking") {
				if (playerEnemyDistance <= ATTACK_START_DISTANCE) {
					//攻撃
					(*it1)->ChengeState(std::make_unique<NormalEnemyAttack>());
				}

			}
			//攻撃
			else if (currentState == "Attack") {
				//攻撃中にプレイヤーが離れた時
				if (playerEnemyDistance > ATTACK_START_DISTANCE) {
					//通常の動き
					(*it1)->ChengeState(std::make_unique<NormalEnemyMove>());
				}
			}

		}
	}

	//強敵の更新
	for (const std::unique_ptr<StrongEnemy>& strongEnemy : strongEnemies_) {
		//一発アウトの敵の更新
		strongEnemy->Update();
		//プレイヤーの座標を設定
		strongEnemy->SetPlayerPosition(playerPosition);

		//AABBの取得
		//通常
		AABB strongEnemyAABB = strongEnemy->GetAABB();
		//強敵
		AABB enemyAABB = strongEnemy->GetAABB();

		//移動中の時
		if (strongEnemy->GetCondition() == EnemyCondition::Move) {
			//レベルエディタから持ってくる
			//座標
			std::vector<Vector3> positions = levelDataManager_->GetObjectPositions(levelDataHandle_,"Stage");
			//AABB
			std::vector<AABB> aabbs = levelDataManager_->GetObjectAABBs(levelDataHandle_,"Stage");
			//コライダーを持っているかどうか
			std::vector<bool> colliders = levelDataManager_->GetIsHavingColliders(levelDataHandle_, "Stage");
			//衝突判定
			for (size_t i = 0; i < positions.size() && i < aabbs.size() && i < colliders.size(); ++i) {


				//AABBを取得
				AABB objectAABB = aabbs[i];
				//位置を取得
				Vector3 objectPosition = positions[i];


				//お互いのAABBが接触している場合
				if (((enemyAABB.max.x > objectAABB.min.x) && (enemyAABB.min.x < objectAABB.max.x)) &&
					((enemyAABB.max.z > objectAABB.min.z) && (enemyAABB.min.z < objectAABB.max.z))) {
					//オブジェクトとの差分ベクトル
					Vector3 defference = VectorCalculation::Subtract(objectPosition, strongEnemy->GetWorldPosition());
					//正規化
					Vector3 normalizedDefference = VectorCalculation::Normalize(defference);


					//敵の向いている方向
					Vector3 enemyDirection = strongEnemy->GetDirection();

					//前にある場合だけ計算
					float dot = SingleCalculation::Dot(enemyDirection, normalizedDefference);

					//進行方向上にあるときだけ計算する
					if (dot > FRONT_DOT_) {

						//差分ベクトルのXとZの大きさを比べ
						//値が大きい方で反転させる
						float defferenceValueX = std::abs(defference.x);
						float defferenceValueZ = std::abs(defference.z);


						//X軸反転
						if (defferenceValueX >= defferenceValueZ) {
							strongEnemy->InvertDirectionX();
						}
						//Z軸反転
						else {
							strongEnemy->InvertDirectionZ();
						}
					}
				}

			}
		}

		//差分を求める
		Vector3 playerStrongEnemyDifference = VectorCalculation::Subtract(playerPosition, strongEnemy->GetWorldPosition());
		//距離を求める
		float playerStrongEnemyDistance = SingleCalculation::Length(playerStrongEnemyDifference);
		//正規化
		Vector3 directionToPlayer = VectorCalculation::Normalize(playerStrongEnemyDifference);


		//大きさの処理
		//追跡開始する距離と強敵とプレイヤーの距離の割合で音量を決める
		const float MAX_VOLUME = 1.0f;
		float volume = MAX_VOLUME - (playerStrongEnemyDistance / STRONG_ENEMY_TRACKING_START_DISTANCE_);


		//0だったら鳴らす意味はないので止めておく
		const float MIN_VOLUME = 0.0f;
		if (volume < MIN_VOLUME) {
			audio_->Stop(audioHandle_);
		}
		else {
			//再生
			audio_->Play(audioHandle_, true);
		}

		//音量変化
		audio_->ChangeVolume(audioHandle_, volume);




#ifdef _DEBUG
		ImGui::Begin("強敵");
		ImGui::InputFloat("距離", &volume);
		ImGui::InputFloat3("プレイヤーとの方向", &directionToPlayer.x);
		ImGui::InputFloat3("プレイヤーとの差分", &playerStrongEnemyDifference.x);
		ImGui::InputFloat("プレイヤーとの距離", &playerStrongEnemyDistance);
		ImGui::End();
#endif // _DEBUG


		//設定した距離より小さくなると追跡
		if (playerStrongEnemyDistance <= STRONG_ENEMY_TRACKING_START_DISTANCE_) {
			//追跡に移行
			strongEnemy->ChangeState(std::make_unique<StrongEnemyTracking>());
		}
		else {
			//通常の動きに移行
			strongEnemy->ChangeState(std::make_unique<StrongEnemyMove>());
		}
	}
}

void EnemyManager::Draw(const Camera& camera,const SpotLight& spotLight){

	//描画(通常)
	for (const std::unique_ptr <NormalEnemy>& enemy : enemies_) {
		enemy->Draw(camera,spotLight);
	}

	//描画(強敵)
	for (const std::unique_ptr<StrongEnemy>&  strongEnemy : strongEnemies_) {
		strongEnemy->Draw(camera, spotLight);
	}

}

void EnemyManager::GenerateNormalEnemy(const Vector3& position) {
	//通常の敵の生成
	std::unique_ptr<NormalEnemy> enemy = std::make_unique<NormalEnemy>();

	//方向決め
	std::random_device seedGenerator;
	std::mt19937 randomEngine(seedGenerator());
	std::uniform_real_distribution<float> speedDistribute(-1.0f, 1.0f);
	Vector3 direction = { .x = speedDistribute(randomEngine),.y = 0.0f,.z = speedDistribute(randomEngine) };

	//初期化
	enemy->Initialize(normalEnemyModelHandle_, position, direction);
	enemies_.push_back(std::move(enemy));
}

void EnemyManager::GenerateStrongEnemy(const Vector3& position) {
	////強敵の生成
	std::unique_ptr<StrongEnemy> enemy = std::make_unique<StrongEnemy>();

	//方向を決める
	std::random_device seedGenerator;
	std::mt19937 randomEngine(seedGenerator());
	std::uniform_real_distribution<float> speedDistribute(-1.0f, 1.0f);
	Vector3 direction = { .x = speedDistribute(randomEngine),.y = 0.0f,.z = speedDistribute(randomEngine) };

	//初期化
	enemy->Initialize(strongEnemyModelHandle_, position, direction);
	enemy->SetTrackingStartDistance(STRONG_ENEMY_TRACKING_START_DISTANCE_);
	//挿入
	strongEnemies_.push_back(std::move(enemy));
}

void EnemyManager::DeleteEnemy() {
	//敵が生存していなかったら消す
	enemies_.remove_if([](const std::unique_ptr<NormalEnemy>& enemy) {
		//スマートポインタの場合はこれだけで良いよ
		//勿論こっちもdeleteが無くなってすっきりだね!
		return enemy->GetIsDeleted();
		});
}

void EnemyManager::StopAudio() {
	audio_->Stop(audioHandle_);
}