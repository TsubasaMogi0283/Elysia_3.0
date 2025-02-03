#include "Key.h"
#include "ModelManager.h"
#include "Easing.h"


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



#ifdef _DEBUG
	//uint32_t debugModelHandle=Ellysia::ModelManager::GetInstance()->LoadModelFile()
	//debugModel_.reset(Ellysia::Model())
	//debugWorldTransform_.Initialize();
#endif // _DEBUG


}

void Key::Update(){
	
	if (isPickUp_ == false) {
		//回転の大きさ
		const float ROTATE_AMOUNT = 0.1f;
		//角度の計算
		theta_ += ROTATE_AMOUNT;
		//上下する
		worldTransform_.translate.y = std::sinf(theta_) * MOVE_AMOUNT_ + originalPositionY_;

		//回転
		worldTransform_.rotate.y += ROTATE_AMOUNT;

	}
	else {
		//上昇回転
		RiseAndRotate();

	}

	
	//ワールドトランスフォームの更新
	worldTransform_.Update();
	//debugWorldTransform_.Update();
	//マテリアルの更新
	material_.Update();

	

}

void Key::Draw(const Camera& camera,const SpotLight& spotLight){
	//本体の描画
	model_->Draw(worldTransform_, camera, material_, spotLight);


#ifdef _DEBUG
	debugModel_->Draw(debugWorldTransform_, camera, material_, spotLight);
#endif // _DEBUG


}

void Key::RiseAndRotate(){
	//イージングを使い急上昇する感じを出す
	//float newT = Easing::EaseOutSine(upT_);
	//newT;
	//高速回転
	const float RAPID_ROTATE_AMOUNT = 0.5f;
	RAPID_ROTATE_AMOUNT;



}


