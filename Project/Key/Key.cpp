#include "Key.h"


void Key::Initialize(const uint32_t& modelhandle,const Vector3& position){
	//モデルの生成
	model_.reset(Model::Create(modelhandle));
	
	//スケールのサイズ
	const float SCALE = 0.4f;
	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	//拡縮
	worldTransform_.scale = {.x = SCALE,.y = SCALE ,.z = SCALE };
	//座標
	worldTransform_.translate = position;
	
	//半径
	//と言っても少しだけ大きくして取りやすくする
	radius_ = SCALE * 4.0f;
	//取得されたかどうかのフラグ
	isPickUp_ = false;

	//マテリアルの初期化
	material_.Initialize();
	//ライティングの種類
	material_.lightingKinds_ = Spot;
	//色
	material_.color_ = {.x = 1.0f,.y = 1.0f,. z = 0.0f,. w = 1.0f };
}

void Key::Update(){
	//ワールドトランスフォームの更新
	worldTransform_.Update();
	
	//マテリアルの更新
	material_.Update();
}

void Key::Draw(const Camera& camera,const SpotLight& spotLight){
	//本体の描画
	model_->Draw(worldTransform_, camera, material_, spotLight);
}

void Key::PickedUp(){
	//拾う
	isPickUp_ = true;
}

