#pragma once
#pragma once
/**
 * @file SkinCluster.h
 * @brief スキンクラスターの構造体
 * @author 茂木翼
 */

#include <vector>
#include <span>

#include "DirectXSetup.h"
#include "Matrix4x4.h"
#include "VertexInfluence.h"
#include "WellForGPU.h"
#include "Skeleton.h"
#include "ModelData.h"
#include "WorldTransform.h"
#include "Camera.h"

/// <summary>
/// スキンクラスター
/// </summary>
struct SkinCluster {

	/// <summary>
	/// SkinClusterを作る
	/// </summary>
	/// <param name="skeleton"></param>
	/// <param name="modelData"></param>
	void Create(const Skeleton& skeleton, const ModelData& modelData);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="skeleton"></param>
	void Update(const Skeleton& skeleton);

	
	//SkinClusterStruct skinClusterStruct_ = {};
	std::vector<Matrix4x4> inverseBindPoseMatrices;

	//Influence
	//頂点に対して影響を与えるパラメータ群
	ComPtr<ID3D12Resource> influenceResource_;
	D3D12_VERTEX_BUFFER_VIEW influenceBufferView_;
	std::span<VertexInfluence>mappedInfluence_;




	//MatrixPalette
	//Skinningを行う際に必要な行列をSkeletonの全Jointの数だけ格納した配列
	ComPtr<ID3D12Resource>paletteResource_;
	std::span<WellForGPU> mappedPalette_;
	std::pair<D3D12_CPU_DESCRIPTOR_HANDLE, D3D12_GPU_DESCRIPTOR_HANDLE> paletteSrvHandle_;
	
	//SRV
	uint32_t srvIndex_;
	
	//スケルトン
	Skeleton skeleton_;
	


};



