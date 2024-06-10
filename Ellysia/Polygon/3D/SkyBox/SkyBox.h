#pragma once

#include "DirectXSetup.h"
#include "ConvertLog.h"

#include "Vector4.h"
#include "Matrix4x4.h"
#include "Matrix4x4Calculation.h"
#include "Transform.h"
#include "VertexData.h"
#include "TransformationMatrix.h"
#include "DirectionalLight.h"

struct WorldTransform;
struct Camera;


class SkyBox {
public:
	
	//コンストラクタ
	SkyBox()=default;

	//初期化
	void Create(uint32_t textureHandle);

	
	//描画
	//左上、右上、左下、右下
	void Draw(WorldTransform & worldTransform,Camera & camera);


	//デストラクタ
	~SkyBox()=default;


private:


	struct SkyBoxMaterial {
		Vector4 color;
	};

	enum VERTEX_POSITION {
		
		//右
		//右上奥
		TOP_RIGHT_BACK,
		//右上前
		TOP_RIGHT_FRONT,
		//右下奥
		BOTTOM_RIGHT_BACK,
		//右下前
		BOTTOM_RIGHT_FRONT,







	};


private:



	//頂点バッファビューを作成する
	D3D12_VERTEX_BUFFER_VIEW vertexBufferViewSphere_;

	//初期化
	//頂点データ
	ComPtr<ID3D12Resource> vertexResource_ = nullptr;
	VertexData* vertexData_ = nullptr;
	const int32_t  SURFACE_VERTEX_= 6;
	uint32_t SURFACE_AMOUNT = 6;


	//index用
	ComPtr<ID3D12Resource> indexResource_ = nullptr;
	//IndexBufferViewを作成
	D3D12_INDEX_BUFFER_VIEW indexBufferView_{};
	//インデックスデータ
	uint32_t* indexData_ = nullptr;


	//マテリアル用のリソースを作る
	ComPtr<ID3D12Resource> materialResource_ = nullptr;
	SkyBoxMaterial* materialData_ = nullptr;


	uint32_t textureHandle_ = 0;

};