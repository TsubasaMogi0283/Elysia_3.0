#pragma once
#include "DirectXSetup.h"
#include "Vector4.h"
#include "Vector3.h"
#include "VertexData.h"

enum TextureEffectType {
	NoneEffect,	//エフェクトなし
	Monochrome,	//白黒(グレースケール)
	Sepia,		//セピア
	Vignette,	//端が暗くなる

	//Smoothing(平滑化)
	//輪郭などのくっきりしたところをぼかして滑らかな雰囲気を出すよ
	BoxFilter3x3,	
	BoxFilter5x5,

	//GaussianFilter
	//BoxFilterよりこっちの方良い感じらしい
	GaussianFilter3x3,
	GaussianFilter5x5,


	//合成させたやつ作りたい

};


struct VignetteInformation {
	//倍
	float scale;
	//乗
	float pow;
	
};

struct GaussianFilterInformation {
	//標準偏差
	//StandardDivision。一般的にσ(シグマ)が使われる
	float sigma;

};


class BackText{
public:
	BackText() {};

	void Initialize();


	void PreDraw();

	void Draw();

	void PostDraw();

	~BackText() {};

private:
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



	uint32_t textureHandle_ = 0;

	D3D12_RESOURCE_BARRIER barrier = {};
};

