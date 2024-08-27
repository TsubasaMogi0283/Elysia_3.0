#include "ObjectManager.h"

void ObjectManager::Initialize(){
	
	uint32_t modelHandle = ModelManager::GetInstance()->LoadModelFile("Resources/Sample/Cube","cube.obj");
	
	//

	DemoObject* demoObject1 = new DemoObject();
	Vector3 demoObjectPosition1 = { .x = 0.0f,.y = 0.0f,.z = 4.0f };
	demoObject1->Initialize(modelHandle, demoObjectPosition1);
	demoObjects_.push_back(demoObject1);



	//uint32_t treeModelHandle = ModelManager::GetInstance()->LoadModelFile("Resources/External/Model/tree_3D","tree1.fbx");
	uint32_t treeModelHandle = ModelManager::GetInstance()->LoadModelFile("Resources/Sample/Cube", "cube.obj");


	Tree* tree1 = new Tree();
	Vector3 treePosition = { 4.0,0.0f,5.0f };
	tree1->Initialize(treeModelHandle, treePosition);
	trees_.push_back(tree1);




	material_.Initialize();
	material_.lightingKinds_ = Spot;
}

void ObjectManager::Update(){
	//DemoObjectの更新
	for (DemoObject* demoObject : demoObjects_) {
		demoObject->Update();
	}

	//木の更新
	for (Tree* tree : trees_) {
		tree->Update();
	}

	//マテリアルの更新
	material_.Update();
}

void ObjectManager::Draw(Camera& camera, SpotLight& spotLight){

	//DemoObjectの描画
	for (DemoObject* demoObject : demoObjects_) {
		demoObject->Draw(camera,material_,spotLight);
	}

	//木の描画
	for (Tree* tree : trees_) {
		tree->Draw(camera,material_,spotLight);
	}

}

ObjectManager::~ObjectManager(){

	//DemoObject
	for (DemoObject* demoObject : demoObjects_) {
		delete demoObject;
	}

	//木
	for (Tree* tree : trees_) {
		delete tree;
	}


}
