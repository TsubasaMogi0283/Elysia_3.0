#include "WorldTransform.h"
#include "Matrix4x4Calculation.h"

#include "DirectXSetup.h"
#include "Camera.h"

void WorldTransform::Initialize() {
	//Resource作成
	bufferResource = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(WorldTransformData)).Get();

	//初期値
	//スケール
	scale = { .x = 1.0f,.y = 1.0f,.z = 1.0f };
	//回転
	rotate = { .x = 0.0f,.y = 0.0f,.z = 0.0f };
	//座標
	translate = { .x = 0.0f,.y = 0.0f,.z = 0.0f };
}


void WorldTransform::Update() {
	//SRT合成
	worldMatrix = Matrix4x4Calculation::MakeAffineMatrix(scale, rotate, translate);

	//逆転置行列
	//ワールド行列を逆転置にする
	Matrix4x4 worldInverseMatrix = Matrix4x4Calculation::Inverse(worldMatrix);
	
	//転置にした
	worldInverseTransposeMatrix = Matrix4x4Calculation::MakeTransposeMatrix(worldInverseMatrix);

	//親があれば親のワールド行列を掛ける
	if (parent) {
		worldMatrix = Matrix4x4Calculation::Multiply(worldMatrix, parent->worldMatrix);
	}

	//転送
	Transfer();
}


void WorldTransform::Transfer() {

	//Resourceに書き込む
	bufferResource->Map(0, nullptr, reinterpret_cast<void**>(&tranceformationData));
	tranceformationData->world = worldMatrix;
	tranceformationData->normal = worldMatrix;
	tranceformationData->worldInverseTranspose = worldInverseTransposeMatrix;
	bufferResource->Unmap(0, nullptr);
}
