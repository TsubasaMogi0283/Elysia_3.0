#include "Camera.h"
#include "Matrix4x4Calculation.h"
#include "WindowsSetup.h"



//初期化
void Camera::Initialize() {
	//Resource作成
	bufferResource_ = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(CameraMatrixData)).Get();

	aspectRatio_ = float(WindowsSetup::GetInstance()->GetClientWidth()) / float(WindowsSetup::GetInstance()->GetClientHeight());

	//初期
	scale_ = { 1.0f, 1.0f,1.0f };
	rotate_ = { 0.0f, 0.0f, 0.0f };
	translate_ = { 0.0f, 0.0f, -9.8f };


	//アフィン行列を計算
	worldMatrix_ = Matrix4x4Calculation::MakeAffineMatrix(scale_, rotate_, translate_);
	//逆行列を計算
	viewMatrix_ = Matrix4x4Calculation::Inverse(worldMatrix_);
	//射影を計算
	projectionMatrix_ = Matrix4x4Calculation::MakePerspectiveFovMatrix(fov_, aspectRatio_, nearClip_, farClip_);
	//正射影行列(正規化)を計算
	orthographicMatrix_ = Matrix4x4Calculation::MakeOrthographicMatrix(0, 0, float(WindowsSetup::GetInstance()->GetClientWidth()), float(WindowsSetup::GetInstance()->GetClientHeight()), 0.0f, 100.0f);

	//転送
	Transfer();
}


void Camera::Update() {

	//アフィン行列を計算
	worldMatrix_ = Matrix4x4Calculation::MakeAffineMatrix(scale_, rotate_, translate_);

	//逆行列を計算
	viewMatrix_ = Matrix4x4Calculation::Inverse(worldMatrix_);
	//射影を計算
	projectionMatrix_ = Matrix4x4Calculation::MakePerspectiveFovMatrix(fov_, aspectRatio_, nearClip_, farClip_);
	//正射影行列(正規化)を計算
	orthographicMatrix_ = Matrix4x4Calculation::MakeOrthographicMatrix(0, 0, float(WindowsSetup::GetInstance()->GetClientWidth()), float(WindowsSetup::GetInstance()->GetClientHeight()), 0.0f, 100.0f);

	//転送
	Transfer();
}

void Camera::Transfer() {
	bufferResource_->Map(0, nullptr, reinterpret_cast<void**>(&cameraMatrixData_));
	cameraMatrixData_->viewMatrix_ = viewMatrix_;
	cameraMatrixData_->projectionMatrix_ = projectionMatrix_;
	cameraMatrixData_->orthographicMatrix_ = orthographicMatrix_;
	bufferResource_->Unmap(0, nullptr);
}