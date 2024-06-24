#include "KeyManager.h"


void KeyManager::Initialize(uint32_t modelHandle){
	Key* key1 = new Key();
	Vector3 keyPosition = { -5.0f,0.0f,1.0f };
	key1->Initialize(modelHandle, keyPosition);
	keyes_.push_back(key1);




	uint32_t keySpriteHandle = TextureManager::GetInstance()->LoadTexture("Resources/Item/KeyList.png");
	Vector3 keySpritePosition = { 0.0f,0.0f,0.0f };
	keySprite_.reset(Sprite::Create(keySpriteHandle, keySpritePosition));

	uint32_t keyNumberQuantity[NUMBER_QUANTITY_] = {};
	for (uint32_t i = 0; i < NUMBER_QUANTITY_; ++i) {
		//数を文字列に変換した方が賢いよね！
		//すっきり！
		const std::string number = std::to_string(i);
		const std::string filePath = "Resources/Number/" + number + ".png";
		keyNumberQuantity[i] = TextureManager::GetInstance()->LoadTexture(filePath);
		const Vector3 numberPosition = { 64.0f * 2.0f,0.0f,0.0f };
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
