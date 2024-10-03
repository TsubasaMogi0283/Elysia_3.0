#pragma once
#include <d3d12.h>
#include <cassert>

#include <CompileShaderManager.h>
#include "ConvertLog.h"
#include <DirectXSetup.h>

//透明化を解決するためにPSOをクラス化しよう
//後ブレンドの時大変だからswitch文で切り替えできるようにしたら楽かも
//スプライトはスプライトで同じ情報だからインスタンス１つでいいよね
//同様にモデルも


enum BlemdMode {
	//ブレンド無し
	BlendModeNone,

	//通常ブレンド
	//Src*SrcA+Dest*(1-SrcA)
	BlendModeNormal,

	//加算ブレンド
	//Src*SrcA+Dest*1
	BlendModeAdd,

	//減算ブレンド
	//Dest*1-Src*SrcA
	BlendModeSubtract,

	//乗算ブレンド
	//Src*0+Dest*Src
	BlendModeMultiply,

	//スクリーンブレンド
	//Src*(1-Dest)+Dest*1
	BlendModeScreen,



};

class PipelineManager final {
private:

	//コンストラクタ
	PipelineManager()=default;

	//デストラクタ
	~PipelineManager()=default;

public:
	//シングルインスタンス
	static PipelineManager* GetInstance();

	//コピーコンストラクタ禁止
	PipelineManager(const PipelineManager& pipelineManager) = delete;

	//代入演算子を無効にする
	PipelineManager& operator=(const PipelineManager& pipelineManager) = delete;

#pragma region アクセッサ

	ComPtr<ID3D12RootSignature> GetLineRootSignature() {
		return linePSO_.rootSignature_;
	}
	ComPtr<ID3D12PipelineState> GetLineGraphicsPipelineState() {
		return linePSO_.graphicsPipelineState_;
	}

	//コマンドに積むためのGetter(Sprite)
	ComPtr<ID3D12RootSignature> GetSpriteRootSignature() {
		return spritePSO_.rootSignature_;
	}
	ComPtr<ID3D12PipelineState> GetSpriteGraphicsPipelineState() {
		return spritePSO_.graphicsPipelineState_;
	}

	//コマンドに積むためのGetter(Model)
	ComPtr<ID3D12RootSignature> GetModelRootSignature() {
		return modelPSO_.rootSignature_;
	}
	ComPtr<ID3D12PipelineState> GetModelGraphicsPipelineState() {
		return modelPSO_.graphicsPipelineState_;
	}


	//コマンドに積む用のGetter(Skinning)
	ComPtr<ID3D12RootSignature> GetAnimationModelRootSignature() {
		return animationModelPSO_.rootSignature_;
	}
	ComPtr<ID3D12PipelineState> GetAnimationModelGraphicsPipelineState() {
		return animationModelPSO_.graphicsPipelineState_;
	}

  //コマンドに積むためのGetter(Particle3D)
	ComPtr<ID3D12RootSignature> GetParticle3DRootSignature() {
		return particle3DPSO_.rootSignature_;
	}
	ComPtr<ID3D12PipelineState> GetParticle3DGraphicsPipelineState() {
		return particle3DPSO_.graphicsPipelineState_;
	}

	//コマンドに積む用のGetter(SkyBox)
	ComPtr<ID3D12RootSignature> GetSkyBoxRootSignature() {
		return skyBoxPSO_.rootSignature_;
	}
	ComPtr<ID3D12PipelineState> GetSkyBoxGraphicsPipelineState() {
		return skyBoxPSO_.graphicsPipelineState_;
	}


	

	//コマンドに積むためのGetter(CopyImage)
	ComPtr<ID3D12RootSignature> GetFullScreenRootSignature() {
		return fullScreenPSO_.rootSignature_;
	}
	ComPtr<ID3D12PipelineState> GetFullScreenGraphicsPipelineState() {
		return fullScreenPSO_.graphicsPipelineState_;
	}

	//コマンドに積むためのGetter(GrayScale)
	ComPtr<ID3D12RootSignature> GetGrayScaleRootSignature() {
		return grayScalePSO_.rootSignature_;
	}
	ComPtr<ID3D12PipelineState> GetGrayScaleGraphicsPipelineState() {
		return grayScalePSO_.graphicsPipelineState_;
	}
	//コマンドに積むためのGetter(SepiaScale)
	ComPtr<ID3D12RootSignature> GetSepiaScaleRootSignature() {
		return sepiaScalePSO_.rootSignature_;
	}
	ComPtr<ID3D12PipelineState> GetSepiaScaleGraphicsPipelineState() {
		return sepiaScalePSO_.graphicsPipelineState_;
	}

	//コマンドに積むためのGetter(Vignette)
	ComPtr<ID3D12RootSignature> GetVignetteRootSignature() {
		return vignettePSO_.rootSignature_;
	}
	ComPtr<ID3D12PipelineState> GetVignetteGraphicsPipelineState() {
		return vignettePSO_.graphicsPipelineState_;
	}
	//コマンドに積むためのGetter(BoxFilter)
	ComPtr<ID3D12RootSignature> GetBoxFilterRootSignature() {
		return boxFilterPSO_.rootSignature_;
	}
	ComPtr<ID3D12PipelineState> GetBoxFilterGraphicsPipelineState() {
		return boxFilterPSO_.graphicsPipelineState_;
	}
	//コマンドに積むためのGetter(GaussianFilter)
	ComPtr<ID3D12RootSignature> GetGaussianFilterRootSignature() {
		return gaussianFilterPSO_.rootSignature_;
	}
	ComPtr<ID3D12PipelineState> GetGaussianFilterGraphicsPipelineState() {
		return gaussianFilterPSO_.graphicsPipelineState_;
	}
	

	//コマンドに積むためのGetter(LuminanceBasedOutline)
	ComPtr<ID3D12RootSignature> GetLuminanceBasedOutlineRootSignature() {
		return luminanceBasedOutlinePSO_.rootSignature_;
	}
	ComPtr<ID3D12PipelineState> GetLuminanceBasedOutlineGraphicsPipelineState() {
		return luminanceBasedOutlinePSO_.graphicsPipelineState_;
	}

	//コマンドに積むためのGetter(DepthBasedOutline)
	ComPtr<ID3D12RootSignature> GetDepthBasedOutlineRootSignature() {
		return depthBasedOutlinePSO_.rootSignature_;
	}
	ComPtr<ID3D12PipelineState> GetDepthBasedOutlineGraphicsPipelineState() {
		return depthBasedOutlinePSO_.graphicsPipelineState_;
	}
	//コマンドに積むためのGetter(RadialBlur)
	ComPtr<ID3D12RootSignature> GetRadialBlurRootSignature() {
		return radialBlurPSO_.rootSignature_;
	}
	ComPtr<ID3D12PipelineState> GetRadialBlurGraphicsPipelineState() {
		return radialBlurPSO_.graphicsPipelineState_;
	}

	//コマンドに積むためのGetter(Dissolve)
	ComPtr<ID3D12RootSignature> GetDissolveRootSignature() {
		return dissolvePSO_.rootSignature_;
	}
	ComPtr<ID3D12PipelineState> GetDissolveGraphicsPipelineState() {
		return dissolvePSO_.graphicsPipelineState_;
	}
	//コマンドに積むためのGetter(RandomEffect)
	ComPtr<ID3D12RootSignature> GetRandomEffectRootSignature() {
		return randomEffectPSO_.rootSignature_;
	}
	ComPtr<ID3D12PipelineState> GetRandomEffectGraphicsPipelineState() {
		return randomEffectPSO_.graphicsPipelineState_;
	}
	




	void SetSpriteBlendMode(uint32_t blendmode) {
		selectSpriteBlendMode_ = blendmode;
	}
	void SetModelBlendMode(uint32_t blendmode) {
		selectModelBlendMode_ = blendmode;
	}




#pragma endregion


private:
	void GenaratePSO(ComPtr<ID3D12RootSignature>, ComPtr<IDxcBlob>,ComPtr<IDxcBlob> vertexShaderBlob, ComPtr<ID3D12PipelineState>);



public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();


	//ライン用
	static void GenaratedLinePSO();

	//スプライト用
	static void GenerateSpritePSO();

	//モデル用
	static void GenerateModelPSO();

	//Skinning
	static void GenerateAnimationModelPSO();

	//3Dパーティクル用
	static void GenerateParticle3DPSO();

	//CopyImage用
	static void GenarateFullScreenPSO();

	//GrayScale用
	static void GenarateGrayScalePSO();

	//SepiaScale用
	static void GenarateSepiaScalePSO();

	//Vignette用
	static void GenarateVignettePSO();

	//BoxFilter用
	static void GenarateBoxFilterPSO();

	//GaussianFilter
	static void GenarateGaussianFilterPSO();

	//OutLine用
	static void GenarateLuminanceBasedOutlinePSO();

	//DepthBasedOutline用
	static void GenarateDepthBasedOutlinePSO();

	//RadialBlur用
	static void GenerateRadialBlurPSO();

	//Dissolve用
	static void GenarateDissolvePSO();

	//RandomEffect用
	static void GenarateRandomEffectPSO();


	//SkyBox
	static void GenarateSkyBoxPSO();

private:

	struct PSOInformation {
		ComPtr<ID3DBlob> signatureBlob_ = nullptr;
		ComPtr<ID3DBlob> errorBlob_ = nullptr;
		ComPtr<ID3D12RootSignature> rootSignature_ = nullptr;
		ComPtr<IDxcBlob> pixelShaderBlob_ = nullptr;
		ComPtr<IDxcBlob> vertexShaderBlob_ = nullptr;
		ComPtr<ID3D12PipelineState> graphicsPipelineState_ = nullptr;

	};

	//ライン用
	PSOInformation linePSO_ = {};
	//スプライト用
	PSOInformation spritePSO_ = {};
	//モデル用の変数
	PSOInformation modelPSO_ = {};
	//モデル用の変数
	PSOInformation particle3DPSO_ = {};

	//CopyImage用
	PSOInformation fullScreenPSO_ = {};
	//GrayScale用
	PSOInformation grayScalePSO_ = {};
	//SepiaScale用
	PSOInformation sepiaScalePSO_ = {};
	//Vignette用
	PSOInformation vignettePSO_ = {};
	//BoxFilter用
	PSOInformation boxFilterPSO_ = {};
	//GaussianFilter用
	PSOInformation gaussianFilterPSO_ = {};
	//LuminanceBasedOutline用
	PSOInformation luminanceBasedOutlinePSO_ = {};
	//DepthBasedOutline用
	PSOInformation depthBasedOutlinePSO_ = {};
	//RadialBlur用
	PSOInformation radialBlurPSO_ = {};
	//Dissolve用
	PSOInformation dissolvePSO_ = {};
	//RandomEffect用
	PSOInformation randomEffectPSO_ = {};

	//アニメーションモデル用の変数
	PSOInformation animationModelPSO_ = {};
	//スカイボックス用の変数
	PSOInformation skyBoxPSO_ = {};



	//スプライト用
	int32_t selectSpriteBlendMode_ = 1;

	//モデル用の
	int32_t selectModelBlendMode_ = 1;

	//モデル用の
	int32_t selectAnimiationModelBlendMode_ = 1;


};