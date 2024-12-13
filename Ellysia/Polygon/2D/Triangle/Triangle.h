#pragma once

/**
 * @file Triangle.h
 * @brief 三角形
 * @author 茂木翼
 */

#include <string>

#include "DirectXSetup.h"

#include "Vector4.h"
#include "Matrix4x4.h"
#include "Matrix4x4Calculation.h"
#include "Transform.h"
#include "VertexData.h"


#include "Material.h"
#include "TransformationMatrix.h"

/// <summary>
/// 三角形
/// </summary>
class Triangle {
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Triangle()=default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="transform"></param>
	/// <param name="color"></param>
	void Draw(Transform transform,Vector4 color);


	/// <summary>
	/// デストラクタ
	/// </summary>
	~Triangle()=default;



private:
	//頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_ = {};

	//Resource
	ComPtr<ID3D12Resource> vertexResouce_;

	//Resourceにデータを書き込む
	VertexData* vertexData_;
	
	//マテリアル用のリソースを作る
	ComPtr<ID3D12Resource> materialResource_ = nullptr;
	MaterialData* materialData_ = nullptr;
	
	//TransformationMatrix用のResource
	ComPtr<ID3D12Resource> wvpResource_ = nullptr;
	TransformationMatrix* wvpData_=nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;


};
