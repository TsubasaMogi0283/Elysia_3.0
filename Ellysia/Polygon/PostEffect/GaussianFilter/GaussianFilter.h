#pragma once
#include "DirectXSetup.h"
#include "Vector4.h"
#include "Vector3.h"
#include "VertexData.h"

enum GaussianFilterSelection {
	//GaussianFilter
	//BoxFilterよりこっちの方良い感じらしい
	GaussianFilter3x3,
	GaussianFilter5x5,



};


struct GaussianFilterData {
	//種類
	int32_t type;
	
	//標準偏差
	//StandardDivision。一般的にσ(シグマ)が使われる
	float sigma;

};


class GaussianFilter {
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	GaussianFilter()=default;


	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 描画前処理
	/// </summary>
	void PreDraw();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GaussianFilter()=default;

private:
	//別々で分けたい
	ComPtr<ID3D12Resource> gaussianFilterResource_ = nullptr;
	GaussianFilterData* gaussianFilterTypeData_ = nullptr;
	int32_t boxFilterType_ = GaussianFilter3x3;
	float sigma_ = 0.0f;

	//RTV
	uint32_t rtvHandle_ = 0;
	ComPtr<ID3D12Resource> rtvResource_ = nullptr;
	//SRV
	uint32_t srvHandle_ = 0;

	D3D12_RESOURCE_BARRIER barrier = {};
};