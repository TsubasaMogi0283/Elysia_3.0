#include "FlashLight.h"

#include <numbers>
#include <imgui.h>

#include "Color.h"
#include "Convert.h"
#include "Camera.h"
#include "ModelManager.h"
#include "GlobalVariables.h"
#include "TextureManager.h"
#include "VectorCalculation.h"
#include "SingleCalculation.h"


FlashLight::FlashLight() {
	//インスタンスの取得
	//グローバル変数クラス
	globalVariables_ = Elysia::GlobalVariables::GetInstance();
	//テクスチャ管理クラス
	textureManager_ = Elysia::TextureManager::GetInstance();
}

void FlashLight::Initialize() {

	//調整項目として扱う
	//光の強さ
	globalVariables_->CreateGroup(FLASH_LIGHT_INTENSITY_STRING_);
	globalVariables_->AddItem(FLASH_LIGHT_INTENSITY_STRING_, MAX_STRING_, maxIntensity_);
	globalVariables_->AddItem(FLASH_LIGHT_INTENSITY_STRING_, MIN_STRING_, minIntensity_);
	//FallowOff
	globalVariables_->CreateGroup(FLASH_LIGHT_COS_FALLOWOFF_START_STRING_);
	globalVariables_->AddItem(FLASH_LIGHT_COS_FALLOWOFF_START_STRING_, MAX_STRING_, maxStart_);
	globalVariables_->AddItem(FLASH_LIGHT_COS_FALLOWOFF_START_STRING_, MIN_STRING_, minStart_);
	//チャージ
	globalVariables_->CreateGroup(FLASH_LIGHT_CHARGE_VALUE_);
	globalVariables_->AddItem(FLASH_LIGHT_CHARGE_VALUE_, CHARGE_INCREASE_STRING_, chargeIncreaseValue_);
	globalVariables_->AddItem(FLASH_LIGHT_CHARGE_VALUE_, CHARGE_DECREASE_STRING_, chargeDecreaseValue_);
	globalVariables_->AddItem(FLASH_LIGHT_CHARGE_VALUE_, CHARGE_GAUGE_SPRITE_POSITION_STRING_, chargeGaugeSpritePosition_);
	globalVariables_->AddItem(FLASH_LIGHT_CHARGE_VALUE_, CHARGE_PARTICLE_RELEASE_TIME_, releaseTime_);

	//スポットライトの初期化
	spotLight_.Initialize();
	spotLight_.position = position_;
	spotLight_.distance = LIGHT_DISTANCE;
	spotLight_.decay = 0.6f;
	spotLight_.cosFallowoffStart = 1.8f;
	spotLight_.intensity = 200.0f;
	spotLight_.aroundOffset = 0.05f;

	//ライトの片方の角度
	//15度=π/12
	lightSideTheta_ = (std::numbers::pi_v<float> / 12.0f);

	//扇
	fan3D_.length = LIGHT_DISTANCE;
	fan3D_.position = position_;
	//同じサイズにする
	fan3D_.sideThetaAngle = lightSideTheta_;
	fan3D_.sidePhiAngleSize = lightSideTheta_;

	//ゲージのスプライトを生成
	uint32_t gaugeTextureHandle = textureManager_->Load("Resources/Sprite/Gauge/Gauge.png");
	chargeGaugeSpritePosition_ = globalVariables_->GetVector2Value(FLASH_LIGHT_CHARGE_VALUE_, CHARGE_GAUGE_SPRITE_POSITION_STRING_);
	chargeGaugeSprite_.reset(Elysia::Sprite::Create(gaugeTextureHandle, chargeGaugeSpritePosition_));
	//チャージの増減値
	chargeIncreaseValue_ = globalVariables_->GetFloatValue(FLASH_LIGHT_CHARGE_VALUE_, CHARGE_INCREASE_STRING_);
	chargeDecreaseValue_= globalVariables_->GetFloatValue(FLASH_LIGHT_CHARGE_VALUE_, CHARGE_DECREASE_STRING_);

	//パーティクルの放出時間
	releaseTime_=globalVariables_->GetFloatValue(FLASH_LIGHT_CHARGE_VALUE_, CHARGE_PARTICLE_RELEASE_TIME_);

	//フレーム
	uint32_t frameSpriteHandle = textureManager_->Load("Resources/Sprite/Gauge/GaugeFrame.png");
	frameSprite_.reset(Elysia::Sprite::Create(frameSpriteHandle, chargeGaugeSpritePosition_));
	//当たり判定の初期化
	flashLightCollision_ = std::make_unique<FlashLightCollision>();
	flashLightCollision_->Initialize();


	
#ifdef _DEBUG

	//マテリアルの初期化
	material_.Initialize();
	material_.lightingKinds = NoneLighting;
	material_.color = { .x = 0.5f,.y = 1.0f,.z = 0.5f,.w = 1.0f };

	//デバッグ用のモデルを生成する
	uint32_t debugModelHandle = Elysia::ModelManager::GetInstance()->LoadModelFile("Resources/Model/Sample/Sphere", "Sphere.obj");

	const float SCALE = 0.4f;
	//左右
	for (uint32_t i = 0; i < SIDE_QUANTITY_; ++i) {
		model_[i].reset(Elysia::Model::Create(debugModelHandle));
		worldTransform_[i].Initialize();
		worldTransform_[i].scale = { .x = SCALE,.y = SCALE ,.z = SCALE };

	}

	//中心
	lightCenterModel_.reset(Elysia::Model::Create(debugModelHandle));
	lightCenterWorldTransform_.Initialize();
	lightCenterWorldTransform_.scale = { .x = SCALE,.y = SCALE ,.z = SCALE };
	lightCenterMaterial_.Initialize();
	lightCenterMaterial_.lightingKinds = NoneLighting;


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
	const float LIGHT_HEIGHT = 0.9f;
	const Vector3 OFFSET = { .x = 0.0f, .y = LIGHT_HEIGHT,.z = 0.0f };
	position_ = VectorCalculation::Add(playerPosition_, OFFSET);

	//計算したものをSpotLightの方に入れる
	spotLight_.position = position_;
	spotLight_.direction = direction_;
	//片方の角度
	spotLight_.cosAngle = std::cosf(lightSideTheta_);

	//割合を求める
	ratio_ = SingleCalculation::InverseLerp(minRange_, maxRange_, lightSideTheta_);

	//幅から強さを計算する
	//最大の強さ
	maxIntensity_ = globalVariables_->GetFloatValue(FLASH_LIGHT_INTENSITY_STRING_, MAX_STRING_);
	//最小の強さ
	minIntensity_ = globalVariables_->GetFloatValue(FLASH_LIGHT_INTENSITY_STRING_, MIN_STRING_);
	spotLight_.intensity = SingleCalculation::Lerp(minIntensity_, maxIntensity_, (1.0f - ratio_));

	//cosFallowoffStart
	//最大
	maxStart_ = globalVariables_->GetFloatValue(FLASH_LIGHT_COS_FALLOWOFF_START_STRING_, MAX_STRING_);
	//最小
	minStart_ = globalVariables_->GetFloatValue(FLASH_LIGHT_COS_FALLOWOFF_START_STRING_, MAX_STRING_);
	spotLight_.cosFallowoffStart = SingleCalculation::Lerp(minStart_, maxStart_, ratio_);

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


	//チャージ
	Charge();

	//当たり判定用へ扇を入力
	flashLightCollision_->SetFan3D(fan3D_);
	//スポットライトの更新
	spotLight_.Update();

	//デバッグ用
	DebugProcess();

}

void FlashLight::DrawObject3D(const Camera& camera) {
#ifdef _DEBUG

	//端
	for (uint32_t i = 0; i < SIDE_QUANTITY_; ++i) {
		model_[i]->Draw(worldTransform_[i], camera, material_, spotLight_);
	}
	//中心
	lightCenterModel_->Draw(lightCenterWorldTransform_, camera, lightCenterMaterial_, spotLight_);

#endif // _DEBUG


	for (const std::unique_ptr <Elysia::Particle3D>& particle : chargeParticle_) {
		if (particle != nullptr) {
			//スポットライトの座標に集まってくるようにする
			particle->Draw(camera, material_);
		}


	}
}

void FlashLight::DrawSprite(){
	//ゲージの描画
	chargeGaugeSprite_->Draw();
	//フレーム
	frameSprite_->Draw();
}

void FlashLight::GenerateParticle() {
	//生成
	std::unique_ptr<Elysia::Particle3D> particle = Elysia::Particle3D::Create(ParticleMoveType::Absorb);

	//パーティクルの細かい設定
	const float SCALE_SIZE = 20.0f;
	particle->SetScale({ .x = SCALE_SIZE,.y = SCALE_SIZE,.z = SCALE_SIZE });
	particle->SetCount(5u);
	particle->SetIsReleaseOnceMode(true);
	particle->SetIsToTransparent(true);
	//挿入
	chargeParticle_.push_back(std::move(particle));
}

void FlashLight::Charge() {
	
	//チャージ中の時
	if (isCharge_ == true) {
		chargeValue_ += chargeIncreaseValue_;
		//最大値を制限
		chargeValue_=std::min<float_t>(MAX_CHARGE_VALUE_, chargeValue_);
		
		//パーティクルの生成
		readyForGenerateParticleTime_ += DELTA_TIME_;
		if (readyForGenerateParticleTime_>releaseTime_) {
			//GenerateParticle();
			readyForGenerateParticleTime_ = 0.0f;
		}
		

		//攻撃できるかどうか
		if (chargeValue_ > isAbleToAttackValue_) {
			isAbleToAttack_ = true;

			//最大値
			if (chargeValue_ >= MAX_CHARGE_VALUE_) {
				//赤
				chargeColor_ = Convert::Color::Adapter(RED);
			}
			else {
				//黄色
				chargeColor_ = Convert::Color::Adapter(YELLOW);
			}
		}
		else {
			//色は緑
			chargeColor_ = Convert::Color::Adapter(GREEN);
			isAbleToAttack_ = false;
		}
	}
	


	//クールタイム
	if (isCoolTime_ == true) {

		chargeValue_ -= chargeDecreaseValue_;
		//最大値を制限
		chargeValue_ = std::min<float_t>(MIN_CHARGE_VALUE_, chargeValue_);
		//青
		chargeColor_ = Convert::Color::Adapter(BLUE);

		//ゲージが0になったら解除
		if (chargeValue_<=0.0f) {
			isCoolTime_ = false;
		}
	}
	


	//値によって伸びる幅が変わる
	chargeGaugeSprite_->SetScale({ .x = chargeValue_,.y = 1.0f });
	chargeGaugeSprite_->SetColor(chargeColor_);

	for (const std::unique_ptr <Elysia::Particle3D>& particle : chargeParticle_) {
		if (particle != nullptr) {
			//スポットライトの座標に集まってくるようにする
			particle->SetAbsorbPosition(spotLight_.position);
		}


	}

}

void FlashLight::ImGuiDisplay() {

	ImGui::Begin("懐中電灯");
	if (ImGui::TreeNode("ライト") == true) {
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
		ImGui::InputFloat("割合", &ratio_);
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("チャージ") == true) {
		ImGui::Checkbox("可能かどうか", &isCharge_);
		ImGui::InputFloat("値", &chargeValue_);
		ImGui::SliderFloat2("ゲージの座標", &chargeGaugeSpritePosition_.x, 0.0f, 1000.0f);
		ImGui::Checkbox("攻撃できるかどうか", &isAbleToAttack_);

		if (ImGui::TreeNode("パーティクル") == true) {
			ImGui::InputFloat("準備時間", &readyForGenerateParticleTime_);
			ImGui::TreePop();
		}

		ImGui::TreePop();
	}


	ImGui::End();
}

void FlashLight::Adjustment() {
	//チャージ座標
	chargeGaugeSpritePosition_ = globalVariables_->GetVector2Value(FLASH_LIGHT_CHARGE_VALUE_, CHARGE_GAUGE_SPRITE_POSITION_STRING_);
	chargeGaugeSprite_->SetPosition(chargeGaugeSpritePosition_);

	//パーティクルの放出時間
	releaseTime_ = globalVariables_->GetFloatValue(FLASH_LIGHT_CHARGE_VALUE_, CHARGE_PARTICLE_RELEASE_TIME_);

	//保存
	globalVariables_->SaveFile(FLASH_LIGHT_INTENSITY_STRING_);
	globalVariables_->SaveFile(FLASH_LIGHT_COS_FALLOWOFF_START_STRING_);
	globalVariables_->SaveFile(FLASH_LIGHT_CHARGE_VALUE_);

}

void FlashLight::DebugProcess(){
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
	worldTransform_[LEFT_].translate = VectorCalculation::Add(playerPosition_, { fanLeft.x ,0.0f,fanLeft.y });
	worldTransform_[RIGHT_].translate = VectorCalculation::Add(playerPosition_, { fanRight.x ,0.0f,fanRight.y });

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


	//ImGuiの表示
	ImGuiDisplay();

	//調整
	Adjustment();
#endif // _EBUG
}
