#include "KeyManager.h"

#include "Audio.h"
#include "TextureManager.h"


KeyManager::KeyManager(){
	//オーディオの取得
	audio_ = Audio::GetInstance();
	//テクスチャ管理クラスの取得
	textureManager_ = TextureManager::GetInstance();
}

void KeyManager::Initialize(const uint32_t& modelHandle){

#pragma region 鍵の生成
	//CSVにしたい
	//1個目生成
	Key* key1 = new Key();
	//座標
	Vector3 keyPosition = { .x = -10.0f,.y = 0.5f,.z = 1.0f };
	//初期化
	key1->Initialize(modelHandle, keyPosition);
	//リストに入れる
	keyes_.push_back(key1);

	//2個目生成
	//以下同様
	Key* key2 = new Key();
	Vector3 keyPosition2 = { .x = 5.0f,.y = 0.5f,.z = 10.0f };
	key2->Initialize(modelHandle, keyPosition2);
	keyes_.push_back(key2);

	//3個目生成
	//以下同様
	Key* key3 = new Key();
	Vector3 keyPosition3 = { .x = 8.0f,.y = 0.5f,.z = -10.0f };
	key3->Initialize(modelHandle, keyPosition3);
	keyes_.push_back(key3);

#pragma endregion

	//読み込み
	uint32_t keySpriteHandle = TextureManager::GetInstance()->LoadTexture("Resources/Item/KeyList.png");
	//座標
	const Vector2 INITIAL_POSITION = {.x= 20.0f,.y=10.0f };
	//鍵の画像の位置
	Vector2 keySpritePosition = { .x= INITIAL_POSITION.x,.y= INITIAL_POSITION.y };
	//生成
	keySprite_.reset(Sprite::Create(keySpriteHandle, keySpritePosition));

	uint32_t keyNumberQuantity[NUMBER_QUANTITY_] = {};
	for (uint32_t i = 0u; i < NUMBER_QUANTITY_; ++i) {
		//数を文字列に変換した方が賢いよね！
		
		//数をstd::stringに変換
		const std::string number = std::to_string(i);
		//変換した番号を組み込む
		const std::string filePath = "Resources/Sprite/Number/" + number + ".png";
		//テクスチャの読み込み
		keyNumberQuantity[i] = TextureManager::GetInstance()->LoadTexture(filePath);
		//座標を決める
		const Vector2 numberPosition = { 64.0f * 2.0f+ INITIAL_POSITION.x,INITIAL_POSITION.y};
		//生成
		keyNumber[i].reset(Sprite::Create(keyNumberQuantity[i], numberPosition));
	}

	//知らせる音の読み込み
	uint32_t notificationSEHandle = audio_->Load("Resources/External/Audio/Key/Shake.mp3");
	//拾う音の読み込み
	uint32_t pickUpSEHandle = audio_->Load("Resources/External/Audio/Key/PickUp.mp3");


}

void KeyManager::Update(){

	//鍵
	for (Key* key : keyes_) {
		//更新
		key->Update();
	}




	keyes_.remove_if([](Key* key) {
		//拾われたら消す
		if (key->GetIsPickUp() == true) {
			delete key;
			return true;
		}
		return false;
	});
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
