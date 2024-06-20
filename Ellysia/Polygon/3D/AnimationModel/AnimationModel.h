#pragma once
#include <cstdint>
#include <ModelData.h>
#include <Camera.h>
#include <SpotLight.h>
#include <PointLight.h>
#include <DirectionalLight.h>

#include  "LightingType.h"
struct WorldTransform;
struct Camera;
struct SkinCluster;

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
	void Draw(WorldTransform& worldTransform, Camera& camera, SkinCluster& skinCluster,DirectionalLight& directionalLight);

	/// <summary>
	/// 描画(点光源)
	/// </summary>
	/// <param name="worldTransform"></param>
	/// <param name="camera"></param>
	/// <param name="skinCluster"></param>
	/// <param name="pointLight"></param>
	void Draw(WorldTransform& worldTransform, Camera& camera, SkinCluster& skinCluster, PointLight& pointLight);


	/// <summary>
	/// 描画(スポットライト)
	/// </summary>
	/// <param name="worldTransform"></param>
	/// <param name="camera"></param>
	/// <param name="skinCluster"></param>
	/// <param name="spotLight"></param>
	void Draw(WorldTransform& worldTransform, Camera& camera, SkinCluster& skinCluster, SpotLight& spotLight);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~AnimationModel() {};


public:
	void SetEviromentTexture(uint32_t textureHandle) {
		this->eviromentTextureHandle_ = textureHandle;
	}


private:
	struct Material {
		Vector4 color;
		int32_t lightingKinds;
		float padding[3];
		Matrix4x4 uvTransform;
		float shininess;
	};


private:
	//頂点リソースを作る
	//頂点バッファビューを作成する
	ComPtr<ID3D12Resource> vertexResource_ = nullptr;
	//頂点バッファビューを作成する
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};

	//インデックス
	ComPtr<ID3D12Resource> indexResource_ = nullptr;
	D3D12_INDEX_BUFFER_VIEW indexBufferView_{};

#pragma region なくしたい


	//マテリアル用のリソースを作る
	ComPtr<ID3D12Resource> materialResource_ = nullptr;
	Material* materialData_ = nullptr;
	//色関係のメンバ変数
	Vector4 materialColor_ = { 1.0f,1.0f,1.0f,1.0f };
	//Ligtingをするかどうか
	//基本はtrueで
	int32_t selectLighting_ = Directional;

	float shininess_ = 100.0f;


#pragma endregion

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

