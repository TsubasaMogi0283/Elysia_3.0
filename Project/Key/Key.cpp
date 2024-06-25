#include "Key.h"




void Key::Initialize(uint32_t& modelhandle, Vector3& position){
	//モデルの生成
	model_.reset(Model::Create(modelhandle));
	
	const float SCALE = 0.4f;
	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	worldTransform_.translate_ = position;
	worldTransform_.scale_ = { SCALE ,SCALE ,SCALE };

	//半径
	//と言っても少しだけ大きくして取りやすくする
	radius_ = SCALE*2.0f;

	//取得されたかどうかのフラグ
	isPickUp_ = false;

	//マテリアルの初期化
	material_.Initialize();
	material_.lightingKinds_ = Spot;
	material_.color_ = { 1.0f,1.0f,0.0f,1.0f };
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
