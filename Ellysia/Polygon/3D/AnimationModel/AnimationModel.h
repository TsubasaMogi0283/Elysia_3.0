#pragma once

/**
 * @file AnimationModel.h
 * @brief アニメーションモデル
 * @author 茂木翼
 */

#include <cstdint>

#include "ModelData.h"
#include "LightingType.h"
#include "Material.h"

#pragma region 前方宣言

/// <summary>
/// ワールドトランスフォーム
/// </summary>
struct WorldTransform;

/// <summary>
/// カメラ
/// </summary>
struct Camera;

/// <summary>
/// GPUに送るカメラ
/// </summary>
struct CameraForGPU;

/// <summary>
/// スキンクラスター
/// </summary>
struct SkinCluster;

/// <summary>
/// 平行光源
/// </summary>
struct DirectionalLight;

/// <summary>
/// 点光源
/// </summary>
struct PointLight;

/// <summary>
/// スポットライト
/// </summary>
struct SpotLight;

/// <summary>
/// マテリアル
/// </summary>
struct Material;

#pragma endregion


/// <summary>
/// アニメーションモデル
/// </summary>
class AnimationModel{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	AnimationModel() {};

	/// <summary>
	/// 生成
	/// </summary>
	/// <param name="modelHandle"></param>
	/// <returns></returns>
	static AnimationModel* Create(uint32_t modelHandle);



	/// <summary>
	/// 描画(平行光源)
	/// </summary>
	/// <param name="worldTransform"></param>
	/// <param name="camera"></param>
	/// <param name="skinCluster"></param>
	/// <param name="directionalLight"></param>
	void Draw(WorldTransform& worldTransform, Camera& camera, SkinCluster& skinCluster, Material& material, DirectionalLight& directionalLight);

	/// <summary>
	/// 描画(点光源)
	/// </summary>
	/// <param name="worldTransform"></param>
	/// <param name="camera"></param>
	/// <param name="skinCluster"></param>
	/// <param name="pointLight"></param>
	void Draw(WorldTransform& worldTransform, Camera& camera, SkinCluster& skinCluster, Material& material, PointLight& pointLight);


	/// <summary>
	/// 描画(スポットライト)
	/// </summary>
	/// <param name="worldTransform"></param>
	/// <param name="camera"></param>
	/// <param name="skinCluster"></param>
	/// <param name="spotLight"></param>
	void Draw(WorldTransform& worldTransform, Camera& camera, SkinCluster& skinCluster, Material& material, SpotLight& spotLight);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~AnimationModel() {};


public:
	void SetEviromentTexture(uint32_t textureHandle) {
		this->eviromentTextureHandle_ = textureHandle;
	}



private:
	//頂点リソースを作る
	//頂点バッファビューを作成する
	ComPtr<ID3D12Resource> vertexResource_ = nullptr;
	//頂点バッファビューを作成する
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};

	//インデックス
	ComPtr<ID3D12Resource> indexResource_ = nullptr;
	D3D12_INDEX_BUFFER_VIEW indexBufferView_{};


	//PixelShaderにカメラの座標を送る為の変数
	ComPtr<ID3D12Resource> cameraResource_ = nullptr;
	CameraForGPU* cameraForGPU_ = {};


	//アニメーションを再生するときに使う時間
	float animationTime_ = 0.0f;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	//環境マップ
	uint32_t eviromentTextureHandle_ = 0;

	//モデルハンドル
	uint32_t modelHandle_ = 0u;
	ModelData modelData_ = {};

	//アニメーションのローカル座標
	//後々シェーダーで渡す
	Matrix4x4 animationLocalMatrix_ = {};

	//デフォルトはα加算
	int32_t blendModeNumber_ = 1;


	bool isAnimation_ = false;


};

