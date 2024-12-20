#pragma once

/**
 * @file Camera.h
 * @brief カメラクラス
 * @author 茂木翼
 */
#include "Matrix4x4.h"
#include "Vector3.h"
#include "DirectXSetup.h"

/// <summary>
/// GPUに送る行列データ
/// </summary>
struct CameraMatrixData {
	//ビュー行列
	Matrix4x4 viewMatrix_;
	//射影行列
	Matrix4x4 projectionMatrix_;
	//正射影行列
	Matrix4x4 orthographicMatrix_;
};

/// <summary>
/// GPUに送る座標データ
/// </summary>
struct CameraForGPU {
	Vector3 worldPosition;
};

/// <summary>
/// カメラ
/// </summary>
struct Camera {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();
public:

	/// <summary>
	/// ワールド座標の取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition() const{
		Vector3 worldPosition = {
			.x = affineMatrix_.m[3][0],
			.y = affineMatrix_.m[3][1],
			.z = affineMatrix_.m[3][2],
		};
		return worldPosition;
	}


private:

	/// <summary>
	/// 転送
	/// </summary>
	void Transfer();

public:
	//リソース
	ComPtr<ID3D12Resource> bufferResource_;


	//角度
	float fov_ = 0.45f;
	//アスペクト比
	float aspectRatio_ = 0.0f;

	//奥行の変数
	float nearClip_ = 0.1f;
	float farClip_ = 1000.0f;


	//回転
	Vector3 rotate_ = {};
	//移動
	Vector3 translate_ = {};


	//アフィン行列
	Matrix4x4 affineMatrix_ = {};
	//ビュー行列
	Matrix4x4 viewMatrix_ = {};
	//射影行列
	Matrix4x4 projectionMatrix_ = {};
	//正射影行列
	Matrix4x4 orthographicMatrix_{};
	//転送用のデータ
	CameraMatrixData* cameraMatrixData_ = nullptr;

private:
	//スケール。
	//本当はいらないけどアフィン行列を作るときに一緒に入れて見やすくしている。
	Vector3 scale_ = {.x= 1.0f,.y= 1.0f,.z= 1.0f };

};