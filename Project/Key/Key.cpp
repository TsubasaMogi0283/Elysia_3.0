#include "Key.h"
#include "ModelManager.h"
#include "Easing.h"
#include "VectorCalculation.h"

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
	uint32_t debugModelHandle = Ellysia::ModelManager::GetInstance()->LoadModelFile("Resources/Model/Sample/Cube", "cube.obj");
	//モデルの生成
	debugModel_.reset(Ellysia::Model::Create(debugModelHandle));
	//ワールドトランスフォームの初期化
	debugWorldTransform_.Initialize();
	//マテリアルの初期化
	debugMaterial_.Initialize();
	debugMaterial_.lightingKinds_ = SpotLighting;
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

	//デバッグ用
	debugWorldTransform_.translate = GetWorldPosition();
	debugWorldTransform_.scale = { .x = 0.1f,.y = 0.1f,.z = 0.1f };
	debugMaterial_.color_.w = 0.3f;

	//ワールドトランスフォームの更新
	worldTransform_.Update();
	debugWorldTransform_.Update();
	//マテリアルの更新
	material_.Update();
	debugMaterial_.Update();

}

void Key::Draw(const Camera& camera,const SpotLight& spotLight){
	//本体の描画
	model_->Draw(worldTransform_, camera, material_, spotLight);


#ifdef _DEBUG
	debugModel_->Draw(debugWorldTransform_, camera, debugMaterial_);
#endif // _DEBUG


}

void Key::RiseAndRotate(){
	//イージングを使い急上昇する感じを出す
	upT_ += 0.01f;
	upT_ = Easing::EaseOutSine(upT_);
	//高さ
	const float HEIGHT = 1.0f;
	//線形補間を使い
	float lerpedPositionY = SingleCalculation::Lerp(GetWorldPosition().y, GetWorldPosition().y+HEIGHT, upT_);
	worldTransform_.translate = GetWorldPosition();
	worldTransform_.translate.y = lerpedPositionY;

	//高速回転
	const float RAPID_ROTATE_AMOUNT = 0.5f;
	worldTransform_.rotate.y += RAPID_ROTATE_AMOUNT;

	//4回転した後に拡縮
	if (worldTransform_.rotate.y > std::numbers::pi_v<float>*12.0f) {
		const float SCALE_DOWM_VALUE = -0.05f;
		const Vector3 SCALE_DOWN = { .x = SCALE_DOWM_VALUE,.y = SCALE_DOWM_VALUE ,.z = SCALE_DOWM_VALUE };
		worldTransform_.scale = VectorCalculation::Add(worldTransform_.scale, SCALE_DOWN);
	}

	//消える
	if (worldTransform_.scale.x < 0.0f&&
		worldTransform_.scale.y < 0.0f&&
		worldTransform_.scale.z < 0.0f) {
		worldTransform_.scale.x = 0.0f;
		worldTransform_.scale.y = 0.0f;
		worldTransform_.scale.z = 0.0f;
		isDelete_ = true;
	}


#ifdef _DEBUG
	ImGui::Begin("KeyLerp"); 
	ImGui::InputFloat("高さ", &lerpedPositionY);
	ImGui::InputFloat3("回転", &worldTransform_.rotate.y);
	ImGui::End();
#endif // _DEBUG


}


