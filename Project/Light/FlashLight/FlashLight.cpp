#include "FlashLight.h"
#include <VectorCalculation.h>
#include "Camera.h"
#include <numbers>
#include <ModelManager.h>

void FlashLight::Initialize(){

	//初期化
	spotLight_.Initialize();
	spotLight_.position_ = lightPosition;
	spotLight_.distance_ = LIGHT_DISTANCE;
	spotLight_.decay_ = 0.6f;
	spotLight_.cosFallowoffStart_ = 6.1f;
	spotLight_.intensity_ = 200.0f;
	spotLight_.aroundOffset_ = 0.1f;

	//ライトの片方の角度
	//15度=π/12
	lightSideTheta = (std::numbers::pi_v<float>/12.0f);
	theta_ = 0.0f;
	phi_ = 0.0f;


	//扇
	fan3D_.length = LIGHT_DISTANCE;
	fan3D_.position = lightPosition;
	//同じサイズにする
	fan3D_.sideThetaAngle = lightSideTheta;
	fan3D_.sidePhiAngleSize = lightSideTheta;



	//マテリアルの初期化
	material_.Initialize();
	material_.lightingKinds_ = None;
	material_.color_ = { 0.5f,1.0f,0.5f,1.0f };


	//デバッグ用のモデルを生成する
	uint32_t modelHandle = ModelManager::GetInstance()->LoadModelFile("Resources/CG3/Sphere", "Sphere.obj");


	const float SCALE = 0.4f;
	//左右
	for (uint32_t i = 0; i < SIDE_QUANTITY_; ++i) {
		model_[i].reset(Model::Create(modelHandle));
		worldTransform_[i].Initialize();
		worldTransform_[i].scale_ = { .x = SCALE,.y = SCALE ,.z = SCALE };

	}

	//中心
	lightCenterModel_.reset(Model::Create(modelHandle));
	lightCenterWorldTransform_.Initialize();
	lightCenterWorldTransform_.scale_ = { .x = SCALE,.y = SCALE ,.z = SCALE };
	lightCenterMaterial_.Initialize();
	lightCenterMaterial_.lightingKinds_ = None;

	//当たり判定の初期化
	flashLightCollision_ = std::make_unique<FlashLightCollision>();
	flashLightCollision_->Initialize();
}

void FlashLight::Update() {

	//ライトの方向ベクトル
	Vector3 direction = {
		.x = std::cosf(theta_),
		.y = 0.0f,
		.z = std::sinf(theta_),
	};

	//上のdirectionから長さを求めてからatan2でyを出す
	float lengthXZ = sqrtf(std::powf(direction.x, 2.0f) + std::powf(direction.z, 2.0f));
	lightDirection_.x = direction.x;
	lightDirection_.y = lengthXZ * std::tanf(phi_);
	lightDirection_.z = direction.z;


	//プレイヤーの座標と微調整分
	//ライトを持つときの高さは地面と同じだと変だよね
	const float LIGHT_HEIGHT = 1.0f;
	const Vector3 OFFSET = { 0.0f, LIGHT_HEIGHT,0.0f };
	lightPosition = VectorCalculation::Add(playerPosition_, OFFSET);

	//計算したものをSpotLightの方に入れる
	spotLight_.position_ = lightPosition;
	spotLight_.direction_ = lightDirection_;

	//片方の角度
	spotLight_.cosAngle_ = std::cosf(lightSideTheta);
	spotLight_.aroundOffset_ = 0.05f;
	
	//扇
	fan3D_.centerRadian = theta_;
	fan3D_.direction = lightDirection_;
	fan3D_.sidePhiAngleSize = lightSideTheta;
	fan3D_.centerPhi = phi_;
	fan3D_.length = spotLight_.distance_;
	fan3D_.position = lightPosition;

	//高さは同じ
	fan3D_.leftVector = { 
		.x = std::cosf(theta_ + lightSideTheta),
		.y = std::sinf(phi_), 
		.z = std::sinf(theta_ + lightSideTheta) 
	};

	fan3D_.rightVector = { 
		.x = std::cosf(theta_ - lightSideTheta),
		.y = std::sinf(phi_), 
		.z = std::sinf(theta_ - lightSideTheta) 
	};




	//端をデバッグ用として表示させる
	const float DISTANCE = 20.0f;
	Vector2 fanLeft = { 
		.x = std::cosf(theta_ + lightSideTheta) * DISTANCE,
		.y = std::sinf(theta_ + lightSideTheta) * DISTANCE 
	};

	Vector2 fanRight = { 
		.x = std::cosf(theta_ - lightSideTheta) * DISTANCE,
		.y = std::sinf(theta_ - lightSideTheta) * DISTANCE 
	};

	
	//端の位置を計算
	worldTransform_[Left].translate_ = VectorCalculation::Add(playerPosition_,{ fanLeft.x ,0.0f,fanLeft.y });
	worldTransform_[Right].translate_ = VectorCalculation::Add(playerPosition_,{ fanRight.x ,0.0f,fanRight.y });

	//中心
	lightCenterWorldTransform_.translate_ = lightPosition;

	//当たり判定用へ扇を入力
	flashLightCollision_->SetFan3D(fan3D_);

#ifdef _DEBUG


	ImGui::Begin("Light");
	ImGui::SliderFloat("Degree", &lightSideTheta,0.0f,90.0f);
	ImGui::InputFloat3("Position", &spotLight_.position_.x);
	ImGui::InputFloat("lengthXZ",&lengthXZ);
	ImGui::InputFloat3("Direction", &spotLight_.direction_.x);
	ImGui::SliderFloat("Distance", &spotLight_.distance_, 0.0f, 100.0f);
	ImGui::SliderFloat("Decay", &spotLight_.decay_, 0.0f, 20.0f);
	ImGui::SliderFloat("FallOff", &spotLight_.cosFallowoffStart_, 0.0f, 20.0f);
	ImGui::SliderFloat("CosAngle", &spotLight_.cosAngle_, 0.0f, 3.0f);
	ImGui::SliderFloat("intencity_", &spotLight_.intensity_, 0.0f, 400.0f);
	ImGui::InputFloat("Theta", &theta_);
	ImGui::InputFloat("Phi", &phi_);
	ImGui::End();
#endif // _DEBUG

	

	//更新
	for (uint32_t i = 0; i < SIDE_QUANTITY_; ++i) {
		worldTransform_[i].Update();
	}
	lightCenterWorldTransform_.Update();
	lightCenterMaterial_.Update();
	material_.Update();
	spotLight_.Update();
}

void FlashLight::Draw(Camera& camera){
#ifdef _DEBUG


	//端
	for (uint32_t i = 0; i < SIDE_QUANTITY_; ++i) {
		model_[i]->Draw(worldTransform_[i], camera, material_, spotLight_);
	}
	//中心
	lightCenterModel_->Draw(lightCenterWorldTransform_, camera, lightCenterMaterial_, spotLight_);

#endif // _DEBUG

	
}
