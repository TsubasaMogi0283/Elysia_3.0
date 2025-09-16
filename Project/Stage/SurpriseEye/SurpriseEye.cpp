#include "SurpriseEye.h"

#include <TextureManager.h>
#include <Audio.h>

SurpriseEye::SurpriseEye(){
	//インスタンス生成
	//テクスチャ管理クラス
	textureManager_ = Elysia::TextureManager::GetInstance();
	//オーディオ
	audio_ = Elysia::Audio::GetInstance();

}


void SurpriseEye::Initialize() {

	//テクスチャ読み込み
	uint32_t textureHandle = textureManager_->Load("Resources/Sprite/Suprise/HorrorEye.jpg");
	//初期座標
	const Vector2 INITIAL_POSITION = { .x = 0.0f,.y = 0.0f };

	//生成
	eye_.reset(Elysia::Sprite::Create(textureHandle, INITIAL_POSITION));
	//非表示
	isInvisible_ = true;
	eye_->SetInvisible(isInvisible_);

	//音の読み込み
	surpriseSeHandle_ = audio_->Load("Resources/Audio/SE/SuprisedEye.wav");
}

void SurpriseEye::Update() {
	//表示中
	if (isInvisible_ == false) {
		//時間を加算
		displayTime_ += DELTA_TIME_;

		//指定した時間を超えたら非表示
		if (displayTime_ > MAX_DISPLAY_TIME_) {
			isInvisible_ = true;
		}
	}

	//非表示の設定
	eye_->SetInvisible(isInvisible_);
}

void SurpriseEye::DrawSprite() {
	//目
	eye_->Draw();
}