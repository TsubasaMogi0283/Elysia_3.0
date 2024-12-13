#pragma once
/**
 * @file Line.h
 * @brief 線のクラス
 * @author 茂木翼
 */

#include <string>

#include "DirectXSetup.h"
#include "ConvertLog.h"
#include "Vector4.h"
#include "Matrix4x4.h"
#include "Matrix4x4Calculation.h"
#include "Material.h"
#include "TransformationMatrix.h"
#include "Camera.h"


/// <summary>
/// 線
/// </summary>
class Line {
public:
	
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Line()=default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="start"></param>
	/// <param name="end"></param>
	/// <param name="camera"></param>
	void Draw(Vector3 start,Vector3 end, Camera& camera);


	/// <summary>
	/// デストラクタ
	/// </summary>
	~Line()=default;

public:

	/// <summary>
	/// 色の設定
	/// </summary>
	/// <param name="color"></param>
	void SetColor(Vector4 color) {
		this->color_ = color;
	}




private:
	struct LineMaterial {
		Vector4 color;
	};


	struct LineTransformMatrix {
		Matrix4x4 WVP;
		Matrix4x4 World;


	};

	//頂点データの拡張
	struct LineVertexData {
		Vector4 position;
	};

private:
	

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_ = {};

	//Resource
	ComPtr<ID3D12Resource> vertexResouce_=nullptr;

	//Resourceにデータを書き込む
	LineVertexData* vertexData_=nullptr;

	//マテリアル用のリソースを作る
	ComPtr<ID3D12Resource> materialResource_ = nullptr;
	

	//TransformationMatrix用のResource
	ComPtr<ID3D12Resource> wvpResource_ = nullptr;
	LineTransformMatrix* wvpData_ = nullptr;


	Vector4 color_ = {};
};

