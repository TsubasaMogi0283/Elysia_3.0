#pragma once
/**
 * @file BoxFilter.h
 * @brief ボックスフィルタのクラス
 * @author 茂木翼
 */


#include "DirectXSetup.h"
#include "Vector4.h"
#include "Vector3.h"
#include "VertexData.h"

/// <summary>
/// ボックスフィルタの種類
/// </summary>
enum BoxFilterSelection {
	BoxFilter3x3,
	BoxFilter5x5,

}; 

/// <summary>
/// 平滑化に関するデータ
/// </summary>
struct BoxFilterType {

	//Smoothing(平滑化)
	//輪郭などのくっきりしたところをぼかして滑らかな雰囲気を出すよ
	int32_t type;

};



/// <summary>
/// ボックスフィルタ
/// </summary>
class BoxFilter {
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	BoxFilter()=default;


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
	~BoxFilter()=default;

private:
	//別々で分けたい
	ComPtr<ID3D12Resource> boxFilterTypeResource_ = nullptr;
	BoxFilterType* boxFilterTypeData_ = nullptr;
	int32_t boxFilterType_ = BoxFilter3x3;

	//RTV
	uint32_t rtvHandle_=0;
	ComPtr<ID3D12Resource> rtvResource_ = nullptr;
	//SRV
	uint32_t srvHandle_ = 0;

	D3D12_RESOURCE_BARRIER barrier = {};
};