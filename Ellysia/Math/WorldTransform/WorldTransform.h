#pragma once

#include "Matrix4x4.h"
#include "Vector3.h"

#include "DirectXSetup.h"

//Wvpいらないかも
//代わりに法線追加
struct WorldTransformData {
	Matrix4x4 world;
	Matrix4x4 normal;
	Matrix4x4 worldInverseTranspose;
};


//Transformationクラスのものをこっちに移動させた方が良いかも
struct WorldTransform {
public:

	//初期化
	void Initialize();

	//行列を計算・転送する
	void Update();

	//ペアレントの設定
	void SetParent(const WorldTransform* parent) {
		parent = parent_;
	}


private:
	//転送
	void Transfer();



public:

	//スケール
	Vector3 scale_ = { 1.0f, 1.0f, 1.0f };
	//回転
	Vector3 rotate_ = { 0.0f, 0.0f, 0.0f };
	//座標
	Vector3 translate_ = { 0.0f, 0.0f, 0.0f };


	//定数バッファ
	ComPtr<ID3D12Resource> bufferResource_;

	WorldTransformData* tranceformationData_ = nullptr;

	//ワールド行列へ
	Matrix4x4 worldMatrix_ = {};
	//逆転置行列
	Matrix4x4 worldInverseTransposeMatrix_ = {};

	//親となるワールド変換へのポインタ
	const WorldTransform* parent_ = nullptr;


};

