#include "KeyManager.h"

void KeyManager::Initialize(uint32_t modelHandle){
	Key* key1 = new Key();
	Vector3 keyPosition = { -5.0f,1.0f,1.0f };
	key1->Initialize(modelHandle, keyPosition);
	keyes_.push_back(key1);

}

void KeyManager::Update(){


	//鍵
	for (Key* key : keyes_) {
		key->Update();
	}

	//鍵の取得処理
	size_t keyQuantity = keyes_.size();
	//鍵が0より多ければ通る
	if (uint32_t(keyQuantity) > 0) {
		//KeyCollision();
	}

	//鍵が取得されたら消す
	keyes_.remove_if([](Key* key) {
		if (key->GetIsPickUp() == true) {
			delete key;
			return true;
		}
		return false;
	});
}

void KeyManager::Draw()
{
}
