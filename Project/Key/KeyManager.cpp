#include "KeyManager.h"


void KeyManager::Initialize(uint32_t modelHandle){


	//CSVにしたい

	Key* key1 = new Key();
	Vector3 keyPosition = { -10.0f,0.5f,1.0f };
	key1->Initialize(modelHandle, keyPosition);
	keyes_.push_back(key1);


	Key* key2 = new Key();
	Vector3 keyPosition2 = { 5.0f,0.5f,10.0f };
	key2->Initialize(modelHandle, keyPosition2);
	keyes_.push_back(key2);

	Key* key3 = new Key();
	Vector3 keyPosition3 = { 8.0f,0.5f,-10.0f };
	key3->Initialize(modelHandle, keyPosition3);
	keyes_.push_back(key3);


	uint32_t keySpriteHandle = TextureManager::GetInstance()->LoadTexture("Resources/Item/KeyList.png");
	const Vector2 INITIAL_POSITION = {.x= 20.0f,.y=10.0f };
	Vector3 keySpritePosition = { .x= INITIAL_POSITION.x,.y= INITIAL_POSITION.y,.z=0.0f };
	keySprite_.reset(Sprite::Create(keySpriteHandle, keySpritePosition));

	uint32_t keyNumberQuantity[NUMBER_QUANTITY_] = {};
	for (uint32_t i = 0; i < NUMBER_QUANTITY_; ++i) {
		//数を文字列に変換した方が賢いよね！
		//すっきり！
		const std::string number = std::to_string(i);
		const std::string filePath = "Resources/Number/" + number + ".png";
		keyNumberQuantity[i] = TextureManager::GetInstance()->LoadTexture(filePath);
		const Vector3 numberPosition = { 64.0f * 2.0f+ INITIAL_POSITION.x,INITIAL_POSITION.y,0.0f };
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
