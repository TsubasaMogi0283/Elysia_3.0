#include "FlashLight.h"
#include <VectorCalculation.h>
#include "Camera.h"
#include <numbers>
#include <ModelManager.h>

void FlashLight::Initialize(){

	//スポットライトの初期化
	spotLight_.Initialize();
	spotLight_.position_ = position_;
	spotLight_.distance_ = LIGHT_DISTANCE;
	spotLight_.decay_ = 0.6f;
	spotLight_.cosFallowoffStart_ = 6.1f;
	spotLight_.intensity_ = 200.0f;
	spotLight_.aroundOffset_ = 0.1f;

	//ライトの片方の角度
	//15度=π/12
	lightSideTheta_ = (std::numbers::pi_v<float>/12.0f);
	theta_ = 0.0f;
	phi_ = 0.0f;


	//扇
	fan3D_.length = LIGHT_DISTANCE;
	fan3D_.position = position_;
	//同じサイズにする
	fan3D_.sideThetaAngle = lightSideTheta_;
	fan3D_.sidePhiAngleSize = lightSideTheta_;

	

	//当たり判定の初期化
	flashLightCollision_ = std::make_unique<FlashLightCollision>();
	flashLightCollision_->Initialize();

#ifdef _DEBUG

	//マテリアルの初期化
	material_.Initialize();
	material_.lightingKinds_ = NoneLighting;
	material_.color_ = { .x = 0.5f,.y = 1.0f,.z = 0.5f,.w = 1.0f };

	//デバッグ用のモデルを生成する
	uint32_t debugModelHandle = Ellysia::ModelManager::GetInstance()->LoadModelFile("Resources/Model/Sample/Sphere", "Sphere.obj");

	const float SCALE = 0.4f;
	//左右
	for (uint32_t i = 0; i < SIDE_QUANTITY_; ++i) {
		model_[i].reset(Ellysia::Model::Create(debugModelHandle));
		worldTransform_[i].Initialize();
		worldTransform_[i].scale = { .x = SCALE,.y = SCALE ,.z = SCALE };

	}

	//中心
	lightCenterModel_.reset(Ellysia::Model::Create(debugModelHandle));
	lightCenterWorldTransform_.Initialize();
	lightCenterWorldTransform_.scale = { .x = SCALE,.y = SCALE ,.z = SCALE };
	lightCenterMaterial_.Initialize();
	lightCenterMaterial_.lightingKinds_ = NoneLighting;


#endif // _DEBUG	
}

void FlashLight::Update() {

	//ライトの方向ベクトル
	Vector3 direction = {
		.x = std::cosf(theta_),
		.y = 0.0f,
		.z = std::sinf(theta_),
	};

	//上のdirectionから長さを求めてからtanfでyを出す
	float lengthXZ = sqrtf(std::powf(direction.x, 2.0f) + std::powf(direction.z, 2.0f));
	direction_ = {
		.x = direction.x,
		.y = lengthXZ * std::tanf(phi_),
		.z = direction.z,
	};


	//プレイヤーの座標と微調整分
	//ライトを持つときの高さは地面と同じだと変だよね
	const float LIGHT_HEIGHT = 1.0f;
	const Vector3 OFFSET = {.x = 0.0f, .y = LIGHT_HEIGHT,.z =  0.0f };
	position_ = VectorCalculation::Add(playerPosition_, OFFSET);

	//計算したものをSpotLightの方に入れる
	spotLight_.position_ = position_;
	spotLight_.direction_ = direction_;

	//片方の角度
	spotLight_.cosAngle_ = std::cosf(lightSideTheta_);
	spotLight_.aroundOffset_ = 0.05f;
	
	//扇
	fan3D_.centerRadian = theta_;
	fan3D_.direction = direction_;
	fan3D_.sidePhiAngleSize = lightSideTheta_;
	fan3D_.centerPhi = phi_;
	fan3D_.length = spotLight_.distance_;
	fan3D_.position = position_;

	//高さは同じ
	//左
	fan3D_.leftVector = { 
		.x = std::cosf(theta_ + lightSideTheta_),
		.y = std::sinf(phi_), 
		.z = std::sinf(theta_ + lightSideTheta_) 
	};
	//右
	fan3D_.rightVector = { 
		.x = std::cosf(theta_ - lightSideTheta_),
		.y = std::sinf(phi_), 
		.z = std::sinf(theta_ - lightSideTheta_) 
	};




	//端をデバッグ用として表示させる
	const float DISTANCE = 20.0f;
	Vector2 fanLeft = { 
		.x = std::cosf(theta_ + lightSideTheta_) * DISTANCE,
		.y = std::sinf(theta_ + lightSideTheta_) * DISTANCE 
	};

	Vector2 fanRight = { 
		.x = std::cosf(theta_ - lightSideTheta_) * DISTANCE,
		.y = std::sinf(theta_ - lightSideTheta_) * DISTANCE 
	};


	//当たり判定用へ扇を入力
	flashLightCollision_->SetFan3D(fan3D_);
	//スポットライトの更新
	spotLight_.Update();
	

#ifdef _DEBUG



	//端の位置を計算
	worldTransform_[Left].translate = VectorCalculation::Add(playerPosition_,{ fanLeft.x ,0.0f,fanLeft.y });
	worldTransform_[Right].translate = VectorCalculation::Add(playerPosition_,{ fanRight.x ,0.0f,fanRight.y });

	//中心
	lightCenterWorldTransform_.translate = position_;

	

	//更新
	//ワールドトランスフォーム
	for (uint32_t i = 0; i < SIDE_QUANTITY_; ++i) {
		worldTransform_[i].Update();
	}
	lightCenterWorldTransform_.Update();

	//マテリアル
	material_.Update();

	lightCenterMaterial_.Update();
	//ImGuiの表示
	ImGuiDisplay();
#endif // _EBUG

}

void FlashLight::Draw(const Camera& camera){
#ifdef _DEBUG


	//端
	for (uint32_t i = 0; i < SIDE_QUANTITY_; ++i) {
		model_[i]->Draw(worldTransform_[i], camera, material_, spotLight_);
	}
	//中心
	lightCenterModel_->Draw(lightCenterWorldTransform_, camera, lightCenterMaterial_, spotLight_);

#endif // _DEBUG

	
}

void FlashLight::ImGuiDisplay(){
	ImGui::Begin("懐中電灯");
	ImGui::SliderFloat("角度", &lightSideTheta_, 0.0f, 3.0f);
	ImGui::InputFloat3("座標", &spotLight_.position_.x);
	ImGui::InputFloat3("方向", &spotLight_.direction_.x);
	ImGui::SliderFloat("距離", &spotLight_.distance_, 0.0f, 100.0f);
	ImGui::SliderFloat("Decay", &spotLight_.decay_, 0.0f, 20.0f);
	ImGui::SliderFloat("FallOff", &spotLight_.cosFallowoffStart_, 0.0f, 3.0f);
	ImGui::SliderFloat("CosAngle", &spotLight_.cosAngle_, 0.0f, 3.0f);
	ImGui::SliderFloat("強さ", &spotLight_.intensity_, 0.0f, 400.0f);
	ImGui::InputFloat("シータ", &theta_);
	ImGui::InputFloat("ファイ", &phi_);
	ImGui::End();
}
