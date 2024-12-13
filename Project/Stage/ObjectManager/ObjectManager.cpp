#include "ObjectManager.h"
#include <ModelManager.h>

void ObjectManager::Initialize(){
	
#pragma region 木
	//uint32_t treeModelHandle = ModelManager::GetInstance()->LoadModelFile("Resources/External/Model/tree_3D","tree1.fbx");
	uint32_t trunkModelHandle = ModelManager::GetInstance()->LoadModelFile("Resources/External/Model/tree_3D/Tree1", "Trunk1.obj");

	//木1
	StageObjectPre* tree1 = new Tree();
	Vector3 treePosition = {.x= 4.0,.y= 0.0f,.z= 5.0f };
	tree1->Initialize(trunkModelHandle, treePosition);
	stageObjects_.push_back(tree1);
	
	//木2
	StageObjectPre* tree2 = new Tree();
	Vector3 treePosition2 = {.x= -4.0,.y= 0.0f,.z =-8.0f };
	tree2->Initialize(trunkModelHandle, treePosition2);
	stageObjects_.push_back(tree2);

#pragma endregion

	//マテリアルの初期化
	material_.Initialize();
	//ライティングの設定
	material_.lightingKinds_ = Spot;
}

void ObjectManager::Update(){
	//更新
	for (StageObjectPre* stageObject : stageObjects_) {
		stageObject->Update();
	}

	//マテリアルの更新
	material_.Update();
}

void ObjectManager::Draw(Camera& camera, SpotLight& spotLight){

	//ステージオブジェクト
	for (StageObjectPre* stageObject : stageObjects_) {
		stageObject->Draw(camera, material_, spotLight);
	}

}

ObjectManager::~ObjectManager(){

	//全て消去
	for (StageObjectPre* stageObject : stageObjects_) {
		delete stageObject;
	}


}
