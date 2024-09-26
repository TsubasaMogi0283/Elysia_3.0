#include "ObjectManager.h"

void ObjectManager::Initialize(){
	
	
#pragma region デモ用
	//uint32_t modelHandle = ModelManager::GetInstance()->LoadModelFile("Resources/Sample/Cube", "cube.obj");
	//
	//StageObject* demoObject1 = new DemoObject();
	//Vector3 demoObjectPosition1 = { .x = 0.0f,.y = 0.0f,.z = 4.0f };
	//demoObject1->Initialize(modelHandle, demoObjectPosition1);
	//stageObjects_.push_back(demoObject1);
	//
	//StageObject* demoObject2 = new DemoObject();
	//Vector3 demoObjectPosition2 = { .x = -5.0f,.y = 0.0f,.z = 4.0f };
	//demoObject2->Initialize(modelHandle, demoObjectPosition2);
	//stageObjects_.push_back(demoObject2);

#pragma endregion


		
#pragma region 木
	//uint32_t treeModelHandle = ModelManager::GetInstance()->LoadModelFile("Resources/External/Model/tree_3D","tree1.fbx");
	uint32_t trunkModelHandle = ModelManager::GetInstance()->LoadModelFile("Resources/External/Model/tree_3D/Tree1", "Trunk1.obj");

	
	StageObject* tree1 = new Tree();
	Vector3 treePosition = { 4.0,0.0f,5.0f };
	tree1->Initialize(trunkModelHandle, treePosition);
	stageObjects_.push_back(tree1);
	
	StageObject* tree2 = new Tree();
	Vector3 treePosition2 = { -4.0,0.0f,-8.0f };
	tree2->Initialize(trunkModelHandle, treePosition2);
	stageObjects_.push_back(tree2);

#pragma endregion

	material_.Initialize();
	material_.lightingKinds_ = Spot;
}

void ObjectManager::Update(){
	//
	for (StageObject* stageObject : stageObjects_) {
		stageObject->Update();
	}

	//マテリアルの更新
	material_.Update();
}

void ObjectManager::Draw(Camera& camera, SpotLight& spotLight){

	//ステージオブジェクト
	for (StageObject* stageObject : stageObjects_) {
		stageObject->Draw(camera, material_, spotLight);
	}

}

ObjectManager::~ObjectManager(){


	for (StageObject* stageObject : stageObjects_) {
		delete stageObject;
	}


}
