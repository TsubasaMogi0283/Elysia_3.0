#include "EnemyManager.h"
#include <random>
#include <cassert>

#include "Player/Player.h"
#include "VectorCalculation.h"
#include "SingleCalculation.h"

#include "Input.h"
#include "Audio.h"
#include "LevelDataManager.h"


#include "StrongEnemy/State/StrongEnemyNoneMove.h"
#include "StrongEnemy/State/StrongEnemyMove.h"
#include "StrongEnemy/State/StrongEnemyTracking.h"

#include "NormalEnemy/State/NormalEnemyMove.h"
#include "NormalEnemy/State/NormalEnemyPreTracking.h"

EnemyManager::EnemyManager(){
	//インスタンスの取得
	//レベルデータ管理クラス
	levelDataManager_ = Ellysia::LevelDataManager::GetInstance();
	//オーディオ
	audio_ = Ellysia::Audio::GetInstance();
}


void EnemyManager::Initialize(const uint32_t& normalEnemyModel,const uint32_t& strongEnemyModel, const std::string& csvPath){
	
	//空だったら引っかかるようにしている
	assert(player_!=nullptr);
	assert(levelDataManager_ != nullptr);


	//モデルハンドルを代入
	//通常
	normalEnemyModelHandle_ = normalEnemyModel;
	//強敵
	strongEnemyModelHandle_ = strongEnemyModel;



	//CSVManagerクラスとか作ってまとめた方が賢いかも
	//まとめたいね
	std::ifstream file;
	file.open(csvPath);
	//開かなかったら止まる
	assert(file.is_open());

	//ファイルの内容を文字列ストリームにコピー
	enemyPositionsFromCSV_ << file.rdbuf();
	//ファイルを閉じる
	file.close();

	//1行分の文字列を入れる変数
	std::string line;


	//CSV読み込み
	while (std::getline(enemyPositionsFromCSV_, line)) {

		//1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream lineStream(line);

		std::string word;
		//,区切りで行の先頭文字列を取得
		std::getline(lineStream, word, ',');

		//「//」があった行の場合コメントなので飛ばす
		if (word.find("//") == 0) {
			//コメントは飛ばす
			continue;
		}


		//通常の敵の場合
		if (word.find("NormalEnemy") == 0) {
			Vector3 position = {};
			//X座標
			std::getline(lineStream, word, ',');
			position.x = static_cast<float>(std::atof(word.c_str()));

			//Y座標
			std::getline(lineStream, word, ',');
			position.y = static_cast<float>(std::atof(word.c_str()));

			//Z座標
			std::getline(lineStream, word, ',');
			position.z = static_cast<float>(std::atof(word.c_str()));

			//生成
			GenerateNormalEnemy(position);

		}
		//強敵の場合
		else if (word.find("StrongEnemy") == 0) {
			Vector3 position = {};
			//X座標
			std::getline(lineStream, word, ',');
			position.x = static_cast<float>(std::atof(word.c_str()));

			//Y座標
			std::getline(lineStream, word, ',');
			position.y = static_cast<float>(std::atof(word.c_str()));

			//Z座標
			std::getline(lineStream, word, ',');
			position.z = static_cast<float>(std::atof(word.c_str()));

			//生成
			//GenerateStrongEnemy(position);

		}


	}

	//接近BGMの設定
	audioHandle_ = audio_->Load("Resources/Audio/Enemy/TrackingToPlayer.mp3");
}

void EnemyManager::DeleteEnemy(){
	//敵が生存していなかったら消す
	enemies_.remove_if([](const std::unique_ptr<NormalEnemy>& enemy) {
		//スマートポインタの場合はこれだけで良いよ
		//勿論こっちもdeleteが無くなってすっきりだね!
		return enemy->GetIsDeleted();
	});
}

void EnemyManager::StopAudio(){
	audio_->Stop(audioHandle_);
}

void EnemyManager::GenerateNormalEnemy(const Vector3& position) {
	//通常の敵の生成
	std::unique_ptr<NormalEnemy> enemy = std::make_unique<NormalEnemy>();
	std::random_device seedGenerator;
	std::mt19937 randomEngine(seedGenerator());
	std::uniform_real_distribution<float> speedDistribute(-0.001f, 0.001f);

	//スピード決め
	Vector3 speed = { .x = speedDistribute(randomEngine),.y = 0.0f,.z = speedDistribute(randomEngine) };

	//初期化
	enemy->Initialize(normalEnemyModelHandle_, position,speed );
	enemies_.push_back(std::move(enemy));
}

void EnemyManager::GenerateStrongEnemy(const Vector3& position){
	////強敵の生成
	std::unique_ptr<StrongEnemy> enemy = std::make_unique<StrongEnemy>();
	std::random_device seedGenerator;
	std::mt19937 randomEngine(seedGenerator());
	
	//スピード(方向)を決める
	std::uniform_real_distribution<float> speedDistribute(-1.0f, 1.0f);
	Vector3 speed = {.x= speedDistribute(randomEngine),.y = 0.0f,.z = speedDistribute(randomEngine) };
	
	//初期化
	enemy->Initialize(strongEnemyModelHandle_, position, speed);
	enemy->SetTrackingStartDistance(STRONG_ENEMY_TRACKING_START_DISTANCE_);
	//挿入
	strongEnemies_.push_back(std::move(enemy));
}

void EnemyManager::Update(){

	//接近するときの距離
	const float TRACKING_START_DISTANCE_ = 15.0f;
	//攻撃するときの距離
	const float ATTACK_START_DISTANCE_ = 6.0f;
	//プレイヤーの座標
	Vector3 playerPosition = player_->GetWorldPosition();
	//最小の距離
	const float MINIMUM_DISTANCE = 2.0f;
	
	//通常の敵
	for (const std::unique_ptr <NormalEnemy>& enemy : enemies_) {
		//プレイヤーの位置を設定
		enemy->SetPlayerPosition(playerPosition);
		//更新
		enemy->Update();
		//AABB
		AABB enemyAABB = enemy->GetAABB();

		//移動中の時
		if (enemy->GetCondition() == EnemyCondition::Move) {

			//レベルエディタから持ってくる
			//座標
			std::vector<Vector3> positions = levelDataManager_->GetStageObjectPositions(levelDataHandle_);
			//AABB
			std::vector<AABB> aabbs = levelDataManager_->GetStageObjectAABBs(levelDataHandle_);
			//コライダーを持っているかどうか
			std::vector<bool> colliders = levelDataManager_->GetIsHavingColliders(levelDataHandle_,"Stage");
			
			//衝突めり込み防止処理
			for (size_t i = 0; i < positions.size() && i < aabbs.size() && i < colliders.size(); ++i) {

				//AABBを取得
				AABB objectAABB = aabbs[i];
				//位置を取得
				Vector3 objectPosition = positions[i];


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

					}
				}
			}
		}
	}
	
	//現在の敵の数
	uint32_t enemyAmount = static_cast<uint32_t>(enemies_.size());
	//1体だけの時
	const uint32_t ONLY_ONE = 1u;

	//敵同士の判定をやる必要が無いからね
	if (enemyAmount == ONLY_ONE) {
		for (const std::unique_ptr <NormalEnemy>& enemy : enemies_) {
			//状態
			uint32_t condition = enemy->GetCondition();
			std::string currentState = enemy->GetCurrentStateName();
			//向き
			Vector3 enemyDirection = enemy->GetDirection();
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
				if ((defferenceDistance < TRACKING_START_DISTANCE_ && 
					defferenceDistance>ATTACK_START_DISTANCE_)
					&& currentState=="Move") {
					enemy->ChengeState(std::move(std::make_unique<NormalEnemyPreTracking>()));
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
				////前の状態を保存
				//enemy->SetPreCondition(condition);
				////現在の状態を保存
				//uint32_t newCondition = EnemyCondition::Move;
				//enemy->SetCondition(newCondition);
				//enemy->ChengeState(std::move(std::make_unique<NormalEnemyMove>()));

			}

		}
	}

	//1体より多い時
	if (enemyAmount > ONLY_ONE) {
		for (auto it1 = enemies_.begin(); it1 != enemies_.end(); ++it1) {

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

			for (auto it2 = enemies_.begin(); it2 != enemies_.end(); ++it2) {

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
			std::vector<Vector3> positions = levelDataManager_->GetStageObjectPositions(levelDataHandle_);
			//AABB
			std::vector<AABB> aabbs = levelDataManager_->GetStageObjectAABBs(levelDataHandle_);
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
					if (dot > FRONT_DOT) {

						//差分ベクトルのXとZの大きさを比べ
						//値が大きい方で反転させる
						float defferenceValueX = std::abs(defference.x);
						float defferenceValueZ = std::abs(defference.z);


						//X軸反転
						if (defferenceValueX >= defferenceValueZ) {
							strongEnemy->InvertSpeedX();
						}
						//Z軸反転
						else {
							strongEnemy->InvertSpeedZ();
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
