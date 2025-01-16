#pragma once
/**
 * @file BackText.h
 * @brief ポストエフェクトのクラス
 * @author 茂木翼
 */


#include "DirectXSetup.h"
#include "Vector4.h"
#include "Vector3.h"
#include "VertexData.h"


/// <summary>
/// タイプ
/// </summary>
enum TextureEffectType {
	NoneEffect,	//エフェクトなし
	Monochrome,	//白黒(グレースケール)
	Sepia,		//セピア
	VignetteEffect,	//端が暗くなる
	
	//Smoothing(平滑化)
	//輪郭などのくっきりしたところをぼかして滑らかな雰囲気を出すよ
	BoxFilter3x3a,	
	BoxFilter5x5a,

	//GaussianFilter
	//BoxFilterよりこっちの方良い感じらしい
	GaussianFilter3x3a,
	GaussianFilter5x5a,



};


/// <summary>
/// ビネットに関する情報
/// </summary>
struct VignetteInformation {
	//倍
	float scale;
	//乗
	float pow;
	
};

/// <summary>
/// ガウシアンフィルタに関する情報
/// </summary>
struct GaussianFilterInformation {
	//標準偏差
	//StandardDivision。一般的にσ(シグマ)が使われる
	float sigma;

};

/// <summary>
/// ポストエフェクト(通常から他のものまで)
/// </summary>
class BackText{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	BackText()=default;


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
	~BackText()=default;


public:
	/// <summary>
	/// 色の設定
	/// </summary>
	/// <param name="color"></param>
	void SetColour(const Vector4& color) {
		this->renderTargetClearValue_ = color;
	}

private:
	//別々で分けたい
	ComPtr<ID3D12Resource> effectResource_ = nullptr;

	int32_t effectType_ = NoneEffect;
	int32_t* effectTypeData_ = nullptr;

	//Vignette
	ComPtr<ID3D12Resource> vignetteResource_ = nullptr;
	VignetteInformation vignetteInformation_ = {};
	VignetteInformation* vignetteData_ = nullptr;

	//GaussianFilter
	ComPtr<ID3D12Resource> gaussianFilterResource_ = nullptr;
	GaussianFilterInformation gaussianFilterInformation_ = {};
	GaussianFilterInformation* gaussianFilterData_ = nullptr;


	uint32_t rtvHandle_=0;
	//RTV
	ComPtr<ID3D12Resource> rtvResource_ = nullptr;
	Vector4 renderTargetClearValue_ = {.x= 0.0f,.y= 0.5f,.z= 0.5f,.w= 1.0f };
	uint32_t srvHandle_ = 0;

	//バリア
	D3D12_RESOURCE_BARRIER barrier = {};
};