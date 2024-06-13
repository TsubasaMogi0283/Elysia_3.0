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

class PipelineManager final{
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

	//コマンドに積む用のGetter(Sprite)
	ComPtr<ID3D12RootSignature> GetSpriteRootSignature() {
		return spritePSO_.rootSignature_;
	}
	ComPtr<ID3D12PipelineState> GetSpriteGraphicsPipelineState() {
		return spritePSO_.graphicsPipelineState_;
	}

	//コマンドに積む用のGetter(Model)
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


	//コマンドに積む用のGetter(Particle3D)
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


	

	void SetSpriteBlendMode(uint32_t blendmode) {
		selectSpriteBlendMode_ = blendmode;
	}
	void SetModelBlendMode(uint32_t blendmode) {
		selectModelBlendMode_ = blendmode;
	}


#pragma endregion

public:
	//アロー演算子の時邪魔になってしまうからここにいれよう
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