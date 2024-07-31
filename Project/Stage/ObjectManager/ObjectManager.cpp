#include "ObjectManager.h"

void ObjectManager::Initialize(){
	
	uint32_t modelHandle = ModelManager::GetInstance()->LoadModelFile("Resources/Sample/Cube","cube.obj");
	

	DemoObject* demoObject1 = new DemoObject();
	demoObject1->Initialize(modelHandle);
	demoObjects_.push_back(demoObject1);

	material_.Initialize();
	material_.lightingKinds_ = Spot;
}

void ObjectManager::Update(){
	//DemoObjectの更新
	for (DemoObject* demoObject : demoObjects_) {
		demoObject->Update();
	}

	//マテリアルの更新
	material_.Update();
}

void ObjectManager::Draw(Camera& camera, SpotLight& spotLight){

	//DemoObject更新
	for (DemoObject* demoObject : demoObjects_) {
		demoObject->Draw(camera,material_,spotLight);
	}

}

ObjectManager::~ObjectManager(){

	//DemoObject
	for (DemoObject* demoObject : demoObjects_) {
		delete demoObject;
	}

}
