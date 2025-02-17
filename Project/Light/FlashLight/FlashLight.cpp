#include "FlashLight.h"

#include <numbers>

#include "Camera.h"
#include "ModelManager.h"
#include "GlobalVariables.h"
#include "VectorCalculation.h"
#include "SingleCalculation.h"

FlashLight::FlashLight(){
	//グローバル変数クラス
	globalVariables_ = Ellysia::GlobalVariables::GetInstance();
}

void FlashLight::Initialize(){

	//スポットライトの初期化
	spotLight_.Initialize();
	spotLight_.position = position_;
	spotLight_.distance = LIGHT_DISTANCE;
	spotLight_.decay = 0.6f;
	spotLight_.cosFallowoffStart = 1.8f;
	spotLight_.intensity = 200.0f;
	spotLight_.aroundOffset = 0.1f;

	//ライトの片方の角度
	//15度=π/12
	lightSideTheta_ = (std::numbers::pi_v<float>/12.0f);

	//扇
	fan3D_.length = LIGHT_DISTANCE;
	fan3D_.position = position_;
	//同じサイズにする
	fan3D_.sideThetaAngle = lightSideTheta_;
	fan3D_.sidePhiAngleSize = lightSideTheta_;

	//当たり判定の初期化
	flashLightCollision_ = std::make_unique<FlashLightCollision>();
	flashLightCollision_->Initialize();


	//調整項目として扱う
	globalVariables_->CreateGroup(FLASH_LIGHT_INTENSITY_STRING_);
	globalVariables_->AddItem(FLASH_LIGHT_INTENSITY_STRING_, MAX_INTENSITY_STRING_, maxIntensity_);
	globalVariables_->AddItem(FLASH_LIGHT_INTENSITY_STRING_, MIN_INTENSITY_STRING_, minIntensity_);


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
	spotLight_.position = position_;
	spotLight_.direction = direction_;
	//片方の角度
	spotLight_.cosAngle = std::cosf(lightSideTheta_);
	spotLight_.aroundOffset = 0.05f;
	
	
	//400,50
	//幅から強さを計算する
	maxIntensity_ = globalVariables_->GetFloatValue(FLASH_LIGHT_INTENSITY_STRING_, MAX_INTENSITY_STRING_);
	//ライトの最小の強さ
	minIntensity_ = globalVariables_->GetFloatValue(FLASH_LIGHT_INTENSITY_STRING_, MIN_INTENSITY_STRING_);
	

	//1.5~2.4

	//割合を求める
	ratio = SingleCalculation::InverseLerp(minRange_, maxRange_, lightSideTheta_);
	spotLight_.intensity = SingleCalculation::Lerp(minIntensity_, maxIntensity_, (1.0f-ratio));
	spotLight_.cosFallowoffStart= SingleCalculation::Lerp(minIntensity_, maxIntensity_, (1.0f - ratio));

	//扇
	fan3D_.centerRadian = theta_;
	fan3D_.direction = direction_;
	fan3D_.sidePhiAngleSize = lightSideTheta_;
	fan3D_.centerPhi = phi_;
	fan3D_.length = spotLight_.distance;
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


	//当たり判定用へ扇を入力
	flashLightCollision_->SetFan3D(fan3D_);
	//スポットライトの更新
	spotLight_.Update();

#ifdef _DEBUG
	//端をデバッグ用として表示させる
	//左
	Vector2 fanLeft = { 
		.x = std::cosf(theta_ + lightSideTheta_) * spotLight_.distance,
		.y = std::sinf(theta_ + lightSideTheta_) * spotLight_.distance 
	};
	//右
	Vector2 fanRight = { 
		.x = std::cosf(theta_ - lightSideTheta_) * spotLight_.distance,
		.y = std::sinf(theta_ - lightSideTheta_) * spotLight_.distance 
	};

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

	//マテリアルの更新
	material_.Update();
	lightCenterMaterial_.Update();

	//保存
	globalVariables_->SaveFile(FLASH_LIGHT_INTENSITY_STRING_);

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
	ImGui::InputFloat3("座標", &spotLight_.position.x);
	ImGui::InputFloat3("方向", &spotLight_.direction.x);
	ImGui::SliderFloat("距離", &spotLight_.distance, 0.0f, 100.0f);
	ImGui::SliderFloat("Decay", &spotLight_.decay, 0.0f, 20.0f);
	ImGui::SliderFloat("FallOff", &spotLight_.cosFallowoffStart, 0.0f, 3.0f);
	ImGui::SliderFloat("CosAngle", &spotLight_.cosAngle, 0.0f, 3.0f);
	ImGui::SliderFloat("強さ", &spotLight_.intensity, 0.0f, 400.0f);
	ImGui::InputFloat("シータ", &theta_);
	ImGui::InputFloat("ファイ", &phi_);
	ImGui::InputFloat("割合", &ratio);
	ImGui::End();
}
