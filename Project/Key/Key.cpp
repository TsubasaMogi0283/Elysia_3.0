#include "Key.h"


void Key::Initialize(const uint32_t& modelhandle,const Vector3& position){
	//モデルの生成
	model_.reset(Ellysia::Model::Create(modelhandle));
	
	//スケールのサイズ
	const float SCALE = 0.4f;
	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	//拡縮
	worldTransform_.scale = {.x = SCALE,.y = SCALE ,.z = SCALE };
	//座標
	worldTransform_.translate = position;
	originalPositionY_ = position.y;
	//半径
	//と言っても少しだけ大きくして取りやすくする
	radius_ = SCALE * 4.0f;
	//取得されたかどうかのフラグ
	isPickUp_ = false;

	//マテリアルの初期化
	material_.Initialize();
	//ライティングの種類
	material_.lightingKinds_ = SpotLighting;
	//色
	material_.color_ = {.x = 1.0f,.y = 1.0f,. z = 0.0f,. w = 1.0f };
}

void Key::Update(){
	
	//角度の計算
	theta_ += ROTATE_AMOUNT_;
	//上下する
	worldTransform_.translate.y = std::sinf(theta_)* MOVE_AMOUNT_ + originalPositionY_;

	//回転
	worldTransform_.rotate.y += ROTATE_AMOUNT_;
	
	//ワールドトランスフォームの更新
	worldTransform_.Update();
	
	//マテリアルの更新
	material_.Update();

	//上昇回転
	RiseAndRotate();


}

void Key::Draw(const Camera& camera,const SpotLight& spotLight){
	//本体の描画
	model_->Draw(worldTransform_, camera, material_, spotLight);
}

void Key::RiseAndRotate(){

}


