#include "WorldTransform.h"
#include "Matrix4x4Calculation.h"

#include "DirectXSetup.h"
#include <Camera.h>

void WorldTransform::Initialize() {
	//Resource作成
	bufferResource_ = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(WorldTransformData)).Get();

	//初期
	scale_ = { 1.0f, 1.0f,1.0f };
	rotate_ = { 0.0f, 0.0f, 0.0f };
	translate_ = { 0.0f, 0.0f, 0.0f };
}


void WorldTransform::Update() {
	//SRT合成
	worldMatrix_ = MakeAffineMatrix(scale_, rotate_, translate_);
	//逆転置行列
	//ワールド行列を逆転置にする
	Matrix4x4 worldInverseMatrix = Inverse(worldMatrix_);
	//転置にした
	worldInverseTransposeMatrix_ = MakeTransposeMatrix(worldInverseMatrix);

	//親があれば親のワールド行列を掛ける
	if (parent_) {
		worldMatrix_ = Multiply(worldMatrix_, parent_->worldMatrix_);
	}

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
