#include "EnemyManager.h"
#include <random>
#include <cassert>

#include "Player/Player.h"
#include "Stage/ObjectManager/ObjectManager.h"
#include "VectorCalculation.h"
#include "SingleCalculation.h"
#include "Input.h"


//Enemyのこれからやること
//1.追跡システムを単純化する
//2.ポリモーフィズムを上手く使おう
//3.オブジェクトマネージャーからレベルエディタに変えていく


void EnemyManager::Initialize(const uint32_t& normalEnemyModel,const uint32_t& strongEnemyModel){
	
	//空だったら引っかかるようにしている
	assert(player_!=nullptr);
	assert(objectManager_ != nullptr);

	//Stageの四隅が一つでも同じだったら引っかかるようにしている
	//X軸
	assert((stageRect_.leftBack.x != stageRect_.leftFront.x)||
		(stageRect_.leftBack.x != stageRect_.rightBack.x)||
		(stageRect_.leftBack.x != stageRect_.rightFront.x));

	//Z軸
	assert((stageRect_.leftBack.z != stageRect_.leftFront.z) ||
		(stageRect_.leftBack.z != stageRect_.rightBack.z) ||
		(stageRect_.leftBack.z != stageRect_.rightFront.z));


	//モデルを代入
	//通常
	normalEnemyModelHandle_ = normalEnemyModel;
	//強敵
	strongEnemyModelHandle_ = strongEnemyModel;

	//CSVでやった方が良いかも
	//JSONでも良いよね

	//1体目
	Enemy* enemy1 = new Enemy();
	Vector3 position1 = {.x= 0.0f,.y= 0.0f,.z= 11.0f };
	enemy1->Initialize(normalEnemyModelHandle_, position1, {.x = -0.0f,.y= 0.0f,.z = -0.01f });
	//enemyes_.push_back(enemy1);
	delete enemy1;
	//2体目
	Enemy* enemy2 = new Enemy();
	Vector3 position2 = {.x= -5.0f,.y= 0.0f,.z= 15.0f };
	enemy2->Initialize(normalEnemyModelHandle_, position2, {.x= 0.01f,.y= 0.0f,.z= 0.0f });
	//enemyes_.push_back(enemy2);
	delete enemy2;
	//3体目
	Enemy* enemy3 = new Enemy();
	Vector3 position3 = {.x= -10.0f,.y= 0.0f,.z= 4.0f };
	enemy3->Initialize(normalEnemyModelHandle_, position3, {.x= 0.01f,.y= 0.0f,.z= 0.01f });
	//enemyes_.push_back(enemy3);
	delete enemy3;
	


	//強敵生成
	StrongEnemy* enemy = new StrongEnemy();
	std::random_device seedGenerator;
	std::mt19937 randomEngine(seedGenerator());
	
	//位置を決める
	std::uniform_real_distribution<float> positionDistribute(stageRect_.leftBack.x, stageRect_.rightBack.x);
	Vector3 position = { positionDistribute(randomEngine),0.0f,positionDistribute(randomEngine) };
	
	
	//スピードを決める
	std::uniform_real_distribution<float> speedDistribute(-1.0f, 1.0f);
	Vector3 speed = { speedDistribute(randomEngine),0.0f,speedDistribute(randomEngine) };
#ifdef _DEBUG
	position = {.x= -20.0f,.y= 0.0f,.z= 10.0f };
	speed = {.x= 0.01f,.y= 0.0f,.z= -0.03f };
#endif // _DEBUG

	
	
	//強敵の初期化
	enemy->Initialize(strongEnemyModelHandle_, position, speed);
	//追跡開始距離の設定
	enemy->SetTrackingStartDistance(STRONG_ENEMY_TRACKING_START_DISTANCE_);
	//挿入
	//strongEnemyes_.push_back(enemy);

	delete enemy;

	//接近BGMの設定
	audio_ = Audio::GetInstance();
	audioHandle_ = audio_->Load("Resources/Audio/Enemy/TrackingToPlayer.mp3");
}





void EnemyManager::DeleteEnemy(){
	//敵が生存していなかったら消す
	enemyes_.remove_if([](Enemy* enemy) {
		if (enemy->GetIsDeleted() == true) {
			delete enemy;
			return true;
		}
		return false;
	});
}

void EnemyManager::GenarateEnemy() {
	//通常の敵の生成
	Enemy* enemy = new Enemy();
	std::random_device seedGenerator;
	std::mt19937 randomEngine(seedGenerator());
	std::uniform_real_distribution<float> distribute(-30.0f, 30.0f);

	//位置決め
	Vector3 position1 = {.x= distribute(randomEngine),.y= 0.0f,.z= distribute(randomEngine) };
	//スピード決め
	Vector3 speed = {.x= 0.0f,.y= 0.0f,.z= 0.0f };

	//初期化
	enemy->Initialize(normalEnemyModelHandle_, position1,speed );
	enemyes_.push_back(enemy);
}


void EnemyManager::GenarateStrongEnemy(){
	//強敵の生成
	StrongEnemy* enemy = new StrongEnemy();
	std::random_device seedGenerator;
	std::mt19937 randomEngine(seedGenerator());

	//位置を決める
	std::uniform_real_distribution<float> positionDistribute(stageRect_.leftBack.x, stageRect_.rightBack.x);
	Vector3 position = {.x= positionDistribute(randomEngine),.y= 0.0f,.z= positionDistribute(randomEngine) };
	

	//位置を決める
	std::uniform_real_distribution<float> speedDistribute(-1.0f, 1.0f);
	Vector3 speed = {.x= speedDistribute(randomEngine),.y= 0.0f,.z= speedDistribute(randomEngine) };


#ifdef _DEBUG
	position = {.x= -4.0f,.y= 0.0f,.z= 5.0f };
	speed = {.x= -0.01f,.y= 0.0f,.z= 0.03f };
#endif // _DEBUG


	//初期化
	enemy->Initialize(strongEnemyModelHandle_, position, speed);
	//挿入
	strongEnemyes_.push_back(enemy);
}

void EnemyManager::Update(){
#ifdef _DEBUG
	//Gキーで出す
	if (Input::GetInstance()->IsTriggerKey(DIK_G) == true) {
		GenarateStrongEnemy();
	}

#endif

	//接近するときの距離
	const float TRACKING_START_DISTANCE_ = 15.0f;
	//攻撃するときの距離
	const float ATTACK_START_DISTANCE_ = 6.0f;

	//プレイヤーの座標
	Vector3 playerPosition = player_->GetWorldPosition();
	//最小の距離
	const float MINIMUM_DISTANCE = 2.0f;
	


	//ポリモーフィズムでまとめたい
	//↓↓↓↓↓↓↓↓↓↓↓↓↓↓
	//通常の敵
	for (Enemy* enemy : enemyes_) {
		//プレイヤーの位置を設定
		//使っているのはPreTrackingしかないので
		if (enemy->GetCondition() == EnemyCondition::PreTracking) {
			enemy->SetPlayerPosition(playerPosition);

		}
		
		//更新
		enemy->Update();

		//AABB
		AABB enemyAABB = enemy->GetAABB();

		//移動中の時
		if (enemy->GetCondition() == EnemyCondition::Move) {

			//X軸反転
			if ((enemyAABB.min.x < stageRect_.leftBack.x) || (enemyAABB.max.x > stageRect_.rightBack.x)) {
				enemy->InvertSpeedX();
			}
			//Z軸反転
			if ((enemyAABB.min.z < stageRect_.leftFront.z) || (enemyAABB.max.z > stageRect_.leftBack.z)) {
				enemy->InvertSpeedZ();
			}

			#pragma region ステージオブジェクト

			//仮置きのステージオブジェクト
			std::list<StageObjectPre*>stageObjects=objectManager_->GetStageObjets();
			for (StageObjectPre* stageObject : stageObjects) {

				//AABBを取得
				AABB objectAABB = stageObject->GetAABB();
				//位置を取得
				Vector3 objectPosition = stageObject->GetWorldPosition();

				
				//お互いのAABBが接触している場合
				if (((enemyAABB.max.x > objectAABB.min.x) && (enemyAABB.min.x < objectAABB.max.x)) &&
					((enemyAABB.max.z > objectAABB.min.z) && (enemyAABB.min.z < objectAABB.max.z))) {
					//オブジェクトとの差分ベクトル
					Vector3 defference = VectorCalculation::Subtract(objectPosition, enemy->GetWorldPosition());
					//正規化
					Vector3 normalizedDefference = VectorCalculation::Normalize(defference);


					//敵の向いている方向
					Vector3 enemyDirection = enemy->GetDirection();

					//前にある場合だけ計算
					float dot = SingleCalculation::Dot(enemyDirection, normalizedDefference);

					//進行方向上にあるときだけ計算する
					if (dot > FRONT_DOT) {

						//差分ベクトルのXとZの大きさを比べ
						//値が大きい方で反転させる
						float defferenceValueX = std::abs(defference.x);
						float defferenceValueZ = std::abs(defference.z);


						//X軸反転
						if (defferenceValueX >= defferenceValueZ) {
							enemy->InvertSpeedX();
						}
						//Z軸反転
						else {
							enemy->InvertSpeedZ();
						}



#ifdef _DEBUG
						ImGui::Begin("DemoObjectEnemy");
						ImGui::InputFloat("Dot", &dot);
						ImGui::InputFloat3("defference", &defference.x);
						ImGui::InputFloat("defferenceValueX", &defferenceValueX);
						ImGui::InputFloat("defferenceValueZ", &defferenceValueZ);
						ImGui::End();
#endif // _DEBUG




					}
				}

			}

			#pragma endregion
		}


		if (enemy->GetCondition() == EnemyCondition::Tracking) {
		
			//仮置きのステージオブジェクト
			std::list<StageObjectPre*>stageObjects = objectManager_->GetStageObjets();
			for (StageObjectPre* stageObject : stageObjects) {

				//AABBを取得
				AABB objectAABB = stageObject->GetAABB();

				//位置を取得
				Vector3 objectPosition = stageObject->GetWorldPosition();


				//お互いのAABBが接触している場合
				if (((enemyAABB.max.x > objectAABB.min.x) && (enemyAABB.min.x < objectAABB.max.x)) &&
					((enemyAABB.max.z > objectAABB.min.z) && (enemyAABB.min.z < objectAABB.max.z))) {
					//オブジェクトとの差分ベクトル
					Vector3 defference = VectorCalculation::Subtract(objectPosition, enemy->GetWorldPosition());
					Vector3 normalizedDefference = VectorCalculation::Normalize(defference);


					//敵の向いている方向
					Vector3 enemyDirection = enemy->GetDirection();

					//前にある場合だけ計算
					float dot = SingleCalculation::Dot(enemyDirection, normalizedDefference);

					//進行方向上にあるときだけ計算する
					if (dot > FRONT_DOT) {

						//差分ベクトルのXとZの大きさを比べ
						//値が大きい方で反転させる
						float defferenceValueX = std::abs(defference.x);
						float defferenceValueZ = std::abs(defference.z);


						if (defferenceValueX >= defferenceValueZ) {
							enemy->InvertSpeedX();
						}
						else {
							enemy->InvertSpeedZ();
						}



#ifdef _DEBUG
						ImGui::Begin("DemoObjectEnemy");
						ImGui::InputFloat("Dot", &dot);
						ImGui::InputFloat3("defference", &defference.x);
						ImGui::InputFloat("defferenceValueX", &defferenceValueX);
						ImGui::InputFloat("defferenceValueZ", &defferenceValueZ);
						ImGui::End();
#endif // _DEBUG




					}
				}
			}
		}


	}
	
	//現在の敵の数
	uint32_t enemyAmount = static_cast<uint32_t>(enemyes_.size());

	//1体だけの時
	const uint32_t ONLY_ONE = 1u;


	
	
	//衝突判定をやる必要が無いからね
	if (enemyAmount == ONLY_ONE) {
		for (Enemy* enemy : enemyes_) {
			//状態
			uint32_t condition = enemy->GetCondition();


			//向き
			Vector3 enemyDirection = enemy->GetDirection();

			//それぞれのAABB
			//プレイヤー
			AABB playerAABB = player_->GetAABB();
			//敵
			AABB enemyAABB = enemy->GetAABB();


			//プレイヤーと敵の差分ベクトル
			//対象にするものがプレイヤーなのでプレイヤーから引いてね
			Vector3 defference = VectorCalculation::Subtract(playerPosition, enemy->GetWorldPosition());
			//距離
			float defferenceDistance = SingleCalculation::Length(defference);
			//正射影ベクトル
			Vector3 projectDifference = VectorCalculation::Project(defference, enemyDirection);
			//差分の正規化
			Vector3 normalizedDefference = VectorCalculation::Normalize(defference);
			//内積の計算
			float dot = SingleCalculation::Dot(normalizedDefference, enemyDirection);

			
			

			//後頭部に目がついているわけではないからね
			if (dot > FRONT_DOT) {

				//追跡開始距離以内になったら追跡準備に移行
				if ((defferenceDistance < TRACKING_START_DISTANCE_ && defferenceDistance>ATTACK_START_DISTANCE_)
					&& condition == EnemyCondition::Move) {

					//前の状態を保存
					enemy->SetPreCondition(condition);

					//現在の状態を保存
					uint32_t newCondition = EnemyCondition::PreTracking;
					enemy->SetPlayerPosition(playerPosition);
					enemy->SetCondition(newCondition);
				}

				//追跡している時
				//追跡開始距離より短い時
				if (defferenceDistance <= ATTACK_START_DISTANCE_ && condition == EnemyCondition::Tracking) {
					//前の状態を保存
					enemy->SetPreCondition(condition);

					//現在の状態を保存
					uint32_t newCondition = EnemyCondition::Attack;
					enemy->SetCondition(newCondition);
				}
				//攻撃中にプレイヤーが離れた時
				if (defferenceDistance > ATTACK_START_DISTANCE_ && condition == EnemyCondition::Attack) {
					//前の状態を保存
					enemy->SetPreCondition(condition);

					//現在の状態を保存
					uint32_t newCondition = EnemyCondition::Move;
					enemy->SetCondition(newCondition);
				}
				

			}
			//前方にいなければ強制的にMove
			else {
				//前の状態を保存
				enemy->SetPreCondition(condition);
				//現在の状態を保存
				uint32_t newCondition = EnemyCondition::Move;
				enemy->SetCondition(newCondition);
			}

		}
	}

	//1体より多い時
	if (enemyAmount > ONLY_ONE) {
		for (std::list<Enemy*>::iterator it1 = enemyes_.begin(); it1 != enemyes_.end(); ++it1) {

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

			for (std::list<Enemy*>::iterator it2 = enemyes_.begin(); it2 != enemyes_.end(); ++it2) {

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
			uint32_t condition = (*it1)->GetCondition();

			//プレイヤーとの差分
			Vector3 playerEnemyDifference = VectorCalculation::Subtract(playerPosition, (*it1)->GetWorldPosition());
			//プレイヤーとの距離 
			float playerEnemyDistance = SingleCalculation::Length(playerEnemyDifference);

			



			//前方にいた場合
			if (enemyAndEnemyDot >= FRONT_DOT && condition == EnemyCondition::Move) {
				//接触した場合止まる
				if ((aabb[0].min.x < aabb[1].max.x && aabb[0].max.x > aabb[1].min.x) &&
					(aabb[0].min.y < aabb[1].max.y && aabb[0].max.y > aabb[1].min.y) &&
					(aabb[0].min.z < aabb[1].max.z && aabb[0].max.z > aabb[1].min.z)) {
					uint32_t newCondition = EnemyCondition::NoneMove;
					(*it1)->SaveSpeed();
					//前の状態を保存
					(*it1)->SetPreCondition(condition);
					//状態の変更
					(*it1)->SetCondition(newCondition);
					
					continue;
				}
				//接触していない場合
				else {
					uint32_t newCondition = EnemyCondition::Move;
					
					//前の状態を保存
					(*it1)->SetPreCondition(condition);
					//状態の変更
					(*it1)->SetCondition(newCondition);
				}
			}
			//動いていない場合
			else if (condition == EnemyCondition::NoneMove) {

				//前方にいない場合
				if (enemyAndEnemyDot < 0.8f) {
					uint32_t preCondition = EnemyCondition::NoneMove;
					uint32_t newCondition = EnemyCondition::Move;
					//動き出す
					(*it1)->MoveAgain();

					//前の状態を保存
					(*it1)->SetPreCondition(preCondition);
					//状態の変更
					(*it1)->SetCondition(newCondition);

				}
				

			}
			//前方にいない場合
			else {
				if (playerEnemyDistance > TRACKING_START_DISTANCE_) {
					uint32_t newCondition = EnemyCondition::Move;

					//前の状態を保存
					(*it1)->SetPreCondition(condition);

					//状態の変更
					(*it1)->SetCondition(newCondition);
				}


				//設定した値より短くなったら接近開始
				if (condition == EnemyCondition::Move) {
					if (playerEnemyDistance <= TRACKING_START_DISTANCE_) {
						//前回のMove状態を記録
						uint32_t preCondition = condition;
						(*it1)->SetPreCondition(preCondition);

						//状態を記録
						condition = EnemyCondition::PreTracking;
						(*it1)->SetCondition(condition);
					}
				}

				//追跡の時に
				if (condition == EnemyCondition::Tracking) {
					//Moveへ
					if (playerEnemyDistance > TRACKING_START_DISTANCE_) {
						//前回のMove状態を記録
						uint32_t preCondition = condition;
						(*it1)->SetPreCondition(preCondition);

						//状態を記録
						condition = EnemyCondition::Move;
						(*it1)->SetCondition(condition);

					}

					//設定した値より短くなったら攻撃開始
					if (playerEnemyDistance <= ATTACK_START_DISTANCE_ &&
						MINIMUM_DISTANCE < playerEnemyDistance) {
						//前回のMove状態を記録
						uint32_t preCondition = condition;
						(*it1)->SetPreCondition(preCondition);

						//状態を記録
						condition = EnemyCondition::Attack;
						(*it1)->SetCondition(condition);

					}
				}
			}
		}
	}

	//強敵の更新
	for (StrongEnemy* strongEnemy : strongEnemyes_) {
		//一発アウトの敵の更新
		strongEnemy->Update();
		//プレイヤーの座標を設定
		strongEnemy->SetPlayerPosition(playerPosition);

		//AABBの取得
		AABB strongEnemyAABB = strongEnemy->GetAABB();


		

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
			//audio_->Play(audioHandle_, true);
		}

		//音量変化
		audio_->ChangeVolume(audioHandle_, volume);




		//Panの処理
		//方向からPanを振る
		//基本左右だけなのでX軸成分だけとる
		//まだ不具合があるので後ほど修正
		float pan = directionToPlayer.x;
		pan;
		//audio_->SetPan(audioHandle_, pan);

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
			uint32_t newCondition = EnemyCondition::Tracking;
			strongEnemy->SetCondition(newCondition);
		}
		else {
			//通常の動きに移行
			uint32_t newCondition = EnemyCondition::Move;
			strongEnemy->SetCondition(newCondition);
		}



#pragma region ステージの端に行ったら反転
		//X軸反転
		if ((strongEnemyAABB.min.x < stageRect_.leftBack.x) || 
			(strongEnemyAABB.max.x > stageRect_.rightBack.x)) {
			strongEnemy->InvertSpeedX();
		}
		//Z軸反転
		if ((strongEnemyAABB.min.z < stageRect_.leftFront.z) || 
			(strongEnemyAABB.max.z > stageRect_.leftBack.z)) {
			strongEnemy->InvertSpeedZ();
		}
#pragma endregion

	}
}

void EnemyManager::Draw(const Camera& camera,const SpotLight& spotLight){

	//描画(通常)
	for (Enemy* enemy : enemyes_) {
		enemy->Draw(camera,spotLight);
	}

	//描画(強敵)
	for (StrongEnemy* strongEnemy : strongEnemyes_) {
		strongEnemy->Draw(camera, spotLight);
	}

}

EnemyManager::~EnemyManager(){
	//audio_->Stop(audioHandle_);

	//消す(通常)
	for (Enemy* enemy : enemyes_) {
		delete enemy;
	}

	//消す(強敵)
	for (StrongEnemy* strongEnemy : strongEnemyes_) {
		delete strongEnemy;
	}

	
}



