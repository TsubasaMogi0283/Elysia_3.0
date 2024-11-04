#include "AudioTestPlayer.h"
#include "VectorCalculation.h"

void AudioTestPlayer::Initialize(){
	worldTransform_.Initialize();
}

void AudioTestPlayer::Update(){

#ifdef _DEBUG
	ImGui::Begin("音確認用のプレイヤー");
	ImGui::InputFloat3("位置", &worldTransform_.translate.x);
	ImGui::End();

#endif // _DEBUG
	//方向とスピードを計算
	const float SPEED = 0.4f;
	Vector3 newDirection = VectorCalculation::Multiply(direction_, SPEED);

	//位置の更新
	worldTransform_.translate = VectorCalculation::Add(worldTransform_.translate, newDirection);
	worldTransform_.Update();
}

