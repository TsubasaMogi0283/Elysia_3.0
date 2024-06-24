#include "Key.h"




void Key::Initialize(uint32_t& modelhandle, Vector3& position){
	//モデルの生成
	model_.reset(Model::Create(modelhandle));
	
	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	worldTransform_.translate_ = position;
	
	//半径
	radius_ = 1.0f;

	//取得されたかどうかのフラグ
	isPickUp_ = false;

	//マテリアルの初期化
	material_.Initialize();
	material_.lightingKinds_ = Spot;

}

void Key::Update(){



	//更新
	worldTransform_.Update();
	material_.Update();
}

void Key::Draw(Camera& camera, SpotLight& spotLight){
	model_->Draw(worldTransform_, camera, material_, spotLight);
}

void Key::PickedUp(){
	isPickUp_ = true;
}

Vector3 Key::GetWorldPosition()const{
	Vector3 result = {};
	result.x = worldTransform_.worldMatrix_.m[3][0];
	result.y = worldTransform_.worldMatrix_.m[3][1];
	result.z = worldTransform_.worldMatrix_.m[3][2];

	return result;
}
