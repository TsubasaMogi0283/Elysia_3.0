#include "SampleScene2.h"
#include <TextureManager.h>
#include <Input.h>
#include "SampleScene/SampleScene.h"
#include "GameManager.h"

SampleScene2::SampleScene2(){

}

void SampleScene2::Initialize() {
	uint32_t textureHandle = TextureManager::LoadTexture("Resources/Text/EscapeSucceeded.png");
	text_.reset(Sprite::Create(textureHandle, { .x = 0.0f,.y = 0.0f }));
	text_->SetScale({.x=1.0f,.y=1.0f});


	restrat_ = false;

}

void SampleScene2::Update(GameManager* gameManager){

	XINPUT_STATE joyState{};
	if (Input::GetInstance()->GetJoystickState(joyState) == true) {

		//Bボタンを押したとき
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_B) {
			bTriggerTime_ += 1;

		}
		if ((joyState.Gamepad.wButtons & XINPUT_GAMEPAD_B) == 0) {
			bTriggerTime_ = 0;
		}

		if (bTriggerTime_ == 1) {
			//脱出
#ifdef _DEBUG
			ImGui::Begin("B");
			ImGui::End();
			restrat_ = true;
#endif
		}
	}

	//次のシーンへ
	if (Input::GetInstance()->IsTriggerKey(DIK_SPACE) == true) {
		gameManager->ChangeScene(new SampleScene());
		restrat_ = true;
	}

	//今は
	if (restrat_ == true) {

	}
}

void SampleScene2::PreDrawPostEffectFirst(){

}

void SampleScene2::DrawSpriteBack(){

}

void SampleScene2::DrawObject3D(){
	
}

void SampleScene2::DrawPostEffect(){

}

void SampleScene2::DrawSprite(){
	text_->Draw();
}

SampleScene2::~SampleScene2(){

}
