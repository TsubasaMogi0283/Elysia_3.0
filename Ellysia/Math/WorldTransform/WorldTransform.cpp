#include "WorldTransform.h"
#include "Matrix4x4Calculation.h"

#include "DirectXSetup.h"
#include <Camera.h>

void WorldTransform::Initialize() {
	//Resource作成
	bufferResource_ = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(WorldTransformData)).Get();

	//初期値
	scale_ = { .x = 1.0f,.y = 1.0f,.z = 1.0f };
	rotate_ = { .x = 0.0f,.y = 0.0f,.z = 0.0f };
	translate_ = { .x = 0.0f,.y = 0.0f,.z = 0.0f };
}


void WorldTransform::Update() {
	//SRT合成
	worldMatrix_ = Matrix4x4Calculation::MakeAffineMatrix(scale_, rotate_, translate_);

	//逆転置行列
	//ワールド行列を逆転置にする
	Matrix4x4 worldInverseMatrix = Matrix4x4Calculation::Inverse(worldMatrix_);
	
	//転置にした
	worldInverseTransposeMatrix_ = Matrix4x4Calculation::MakeTransposeMatrix(worldInverseMatrix);

	//親があれば親のワールド行列を掛ける
	if (parent_) {
		worldMatrix_ = Matrix4x4Calculation::Multiply(worldMatrix_, parent_->worldMatrix_);
	}

	//転送
	Transfer();
}


void WorldTransform::Transfer() {
	//Resourceに書き込む
	//今までTransformationに書いていたものをこっちに引っ越す
	bufferResource_->Map(0, nullptr, reinterpret_cast<void**>(&tranceformationData_));
	tranceformationData_->world = worldMatrix_;
	tranceformationData_->normal = worldMatrix_;
	tranceformationData_->worldInverseTranspose = worldInverseTransposeMatrix_;
	bufferResource_->Unmap(0, nullptr);
}
