#include "KeyManager.h"

#include <stdlib.h>


#include "TextureManager.h"
#include "VectorCalculation.h"
#include "Player/Player.h"
#include "SingleCalculation.h"


KeyManager::KeyManager(){

	//オーディオの取得
	audio_ = Ellysia::Audio::GetInstance();
	
	//テクスチャ管理クラスの取得
	textureManager_ = Ellysia::TextureManager::GetInstance();
}

void KeyManager::Initialize(const uint32_t& modelHandle, const std::string& csvPath){
	//プレイヤーが入っているかどうか
	assert(player_ != nullptr);

	//モデルハンドルの代入
	modelHandle_ = modelHandle;


	//CSVManagerクラスとか作ってまとめた方が賢いかも
	std::ifstream file;
	file.open(csvPath);
	//開かなかったら止まる
	assert(file.is_open());

	//ファイルの内容を文字列ストリームにコピー
	enemyPositionsFromCSV << file.rdbuf();
	//ファイルを閉じる
	file.close();

	//1行分の文字列を入れる変数
	std::string line;


	//コマンド実行ループ
	while (std::getline(enemyPositionsFromCSV, line)) {

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


		Vector3 position = {};
		//X座標
		position.x = static_cast<float>(std::atof(word.c_str()));

		//Y座標
		std::getline(lineStream, word, ',');
		position.y = static_cast<float>(std::atof(word.c_str()));

		//Z座標
		std::getline(lineStream, word, ',');
		position.z = static_cast<float>(std::atof(word.c_str()));

		//生成
		Genarate(position);

	}


	//読み込み
	uint32_t keySpriteHandle = textureManager_->LoadTexture("Resources/Item/KeyList.png");
	//座標
	const Vector2 INITIAL_POSITION = {.x= 20.0f,.y=10.0f };
	//鍵の画像の位置
	Vector2 keySpritePosition = { .x= INITIAL_POSITION.x,.y= INITIAL_POSITION.y };
	//生成
	keySprite_.reset(Sprite::Create(keySpriteHandle, keySpritePosition));

	//数字
	uint32_t keyNumberQuantity[NUMBER_QUANTITY_] = {};
	for (uint32_t i = 0u; i < NUMBER_QUANTITY_; ++i) {
		//数を文字列に変換した方が賢いよね！
		//数をstd::stringに変換
		const std::string number = std::to_string(i);
		//変換した番号を組み込む
		const std::string filePath = "Resources/Sprite/Number/" + number + ".png";
		//テクスチャの読み込み
		keyNumberQuantity[i] = textureManager_->LoadTexture(filePath);
		//座標を決める
		const Vector2 numberPosition = { 64.0f * 2.0f+ INITIAL_POSITION.x,INITIAL_POSITION.y};
		//生成
		keyNumber[i].reset(Sprite::Create(keyNumberQuantity[i], numberPosition));
	}

	//知らせる音の読み込み
	notificationSEHandle_ = audio_->Load("Resources/External/Audio/Key/Shake.mp3");
	//拾う音の読み込み
	pickUpSEHandle = audio_->Load("Resources/External/Audio/Key/PickUp.mp3");
	

	//再生
	audio_->Play(notificationSEHandle_, true);
	//初期の音の設定
	const float INITIAL_VOLUME = 0.0f;
	audio_->ChangeVolume(notificationSEHandle_, INITIAL_VOLUME);
}

void KeyManager::Genarate(const Vector3& position){
	//生成
	Key* key = new Key();
	//初期化
	key->Initialize(modelHandle_, position);
	//リストに入れる
	keyes_.push_back(key);
}

void KeyManager::Delete(){

	//消去処理
	//外部の変数を持ってきたいときは[]の中に=を入れよう！
	keyes_.remove_if([=](Key* key) {
		//拾われたら消す
		if (key->GetIsPickUp() == true) {
			audio_->Play(pickUpSEHandle, false);

			delete key;
			return true;
		}
		return false;
	});
}

void KeyManager::StopAudio(){
	audio_->ChangeVolume(notificationSEHandle_, 0.0f);
}

void KeyManager::Update(){

	//全ての要素を消す
	keyAndPlayerDistances_.clear();

	//鍵
	for (Key* key : keyes_) {
		//更新
		key->Update();

		//プレイヤーと鍵の差分
		Vector3 playerAndKeydifference = VectorCalculation::Subtract(player_->GetWorldPosition(), key->GetWorldPosition());
		//距離
		float distance = SingleCalculation::Length(playerAndKeydifference);
		//挿入
		keyAndPlayerDistances_.push_back(distance);
	}

	//全ての要素から一番小さい値を調べる
	auto minIt = std::min_element(keyAndPlayerDistances_.begin(), keyAndPlayerDistances_.end());

	//最短距離を求める
	float closestDistance = 0.0f;
	if (minIt != keyAndPlayerDistances_.end()) {
		closestDistance = (*minIt);
	}

	//最大の距離を超えないようにする
	if (closestDistance > MAX_DISTANCE_) {
		closestDistance = MAX_DISTANCE_;
	}
	
	//求めた値から音量設定をする
	float volume = 1.0f-(closestDistance / MAX_DISTANCE_);
	audio_->ChangeVolume(notificationSEHandle_, volume);


#ifdef _DEBUG
	ImGui::Begin("鍵管理クラス"); 
	ImGui::InputFloat("音量", &volume);
	ImGui::InputFloat("近い距離", &closestDistance);
	ImGui::End();
#endif // _DEBUG




	//現在の鍵の数
	size_t currentKeyQuantity = keyes_.size();
	if (currentKeyQuantity == 0u) {
		audio_->Stop(notificationSEHandle_);
	}


	//消去処理
	Delete();
}

void KeyManager::DrawObject3D(const Camera& camera,const SpotLight& spotLight){
	//鍵モデルの描画
	for (Key* key : keyes_) {
		key->Draw(camera, spotLight);
	}
}

void KeyManager::DrawSprite(const uint32_t& playeresKey){
	//鍵の画像の描画
	keySprite_->Draw();
	//数の描画
	keyNumber[playeresKey]->Draw();


}

KeyManager::~KeyManager(){
	
	//消す
	for (Key* key : keyes_) {
		delete  key;
	}

}

