#include "ObjectManager.h"
#include <ModelManager.h>

void ObjectManager::Initialize(){
	

	//マテリアルの初期化
	material_.Initialize();
	//ライティングの設定
	material_.lightingKinds_ = Spot;
}

void ObjectManager::Update(){
	

	//マテリアルの更新
	material_.Update();
}

void ObjectManager::Draw(Camera& camera, SpotLight& spotLight){

	camera; 
	spotLight;
}

ObjectManager::~ObjectManager(){


}
