#pragma once


#include "Matrix4x4.h"
#include "Vector3.h"
#include "DirectXSetup.h"

struct CameraMatrixData {
	//アフィン行列はいらないかも

	//必要なのはこの3つ
	//ビュー行列
	Matrix4x4 viewMatrix_;
	//射影行列
	Matrix4x4 projectionMatrix_;
	//正射影行列
	Matrix4x4 orthographicMatrix_;
};


struct CameraForGPU {
	Vector3 worldPosition;
};


struct Camera {
public:
	//初期化
	void Initialize();

	//行列を計算
	void Update();


	//転送する
	void Transfer();
public:
	/// <summary>
	/// ワールド座標の取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition();


public:
	//リソース
	ComPtr<ID3D12Resource> bufferResource_;


	//詳しくはMT3で
	//角度
	float fov_ = 0.45f;
	//アスペクト比
	float aspectRatio_ = 0.0f;

	//奥行の変数
	float nearClip_ = 0.1f;
	float farClip_ = 1000.0f;



	//回転
	Vector3 rotate_ = { 0.0f,0.0f,0.0f };
	//移動
	Vector3 translate_ = { 0.0f,0.0f,0.0f };



	//アフィン行列
	Matrix4x4 worldMatrix_ = {};
	//ビュー行列
	Matrix4x4 viewMatrix_ = {};
	//射影行列
	Matrix4x4 projectionMatrix_ = {};
	//正射影行列
	Matrix4x4 orthographicMatrix_{};

	CameraMatrixData* cameraMatrixData_ = nullptr;

private:
	//スケールはっきり言って意味ない
	//だけどMakeAffineにするときこれの方が楽だから入れてる
	Vector3 scale_ = { 1.0f,1.0f,1.0f };

};