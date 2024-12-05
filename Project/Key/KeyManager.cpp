#include "KeyManager.h"


void KeyManager::Initialize(const uint32_t& modelHandle){


	
#pragma region 鍵の生成
	//CSVにしたい
	//1個目生成
	Key* key1 = new Key();
	//座標
	Vector3 keyPosition = { -10.0f,0.5f,1.0f };
	//初期化
	key1->Initialize(modelHandle, keyPosition);
	//リストに入れる
	keyes_.push_back(key1);

	//2個目生成
	//以下同様
	Key* key2 = new Key();
	Vector3 keyPosition2 = { 5.0f,0.5f,10.0f };
	key2->Initialize(modelHandle, keyPosition2);
	keyes_.push_back(key2);

	//3個目生成
	//以下同様
	Key* key3 = new Key();
	Vector3 keyPosition3 = { 8.0f,0.5f,-10.0f };
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


}

void KeyManager::Update(){



	//鍵
	for (Key* key : keyes_) {
		key->Update();
	}


	//当たり判定自体はゲームシーンでやりたい
	//鍵が取得されたら消す
	keyes_.remove_if([](Key* key) {
		if (key->GetIsPickUp() == true) {
			delete key;
			return true;
		}
		return false;
	});
}

void KeyManager::DrawObject3D(Camera& camera, SpotLight& spotLight){
	//鍵
	for (Key* key : keyes_) {
		key->Draw(camera, spotLight);
	}
}

void KeyManager::DrawSprite(uint32_t playeresKey){
	keySprite_->Draw();
	keyNumber[playeresKey]->Draw();


}

KeyManager::~KeyManager(){
	for (Key* key : keyes_) {
		delete  key;
	}

}
