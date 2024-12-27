#pragma once

/**
 * @file Particle3Dr.h
 * @brief パーティクル(3D版)のクラス
 * @author 茂木翼
 */


#include <random>
#include <list>

#include "Camera.h"
#include "Transform.h"
#include "Particle.h"
#include "AccelerationField.h"
#include "TransformationMatrix.h"
#include "Matrix4x4Calculation.h"
#include "VertexData.h"
#include "DirectXSetup.h"

#pragma region 前方宣言

/// <summary>
/// マテリアル
/// </summary>
struct Material;

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
/// モデル管理クラス
/// </summary>
class ModelManager;

/// <summary>
/// テクスチャ管理クラス
/// </summary>
class TextureManager;

/// <summary>
/// SRV管理クラス
/// </summary>
class SrvManager;

/// <summary>
/// パイプライン管理クラス
/// </summary>
class PipelineManager;


#pragma endregion


/// <summary>
/// 動き方の設定
/// </summary>
enum ParticleMoveType {

	//通常の放出
	NormalRelease,
	//鉛直投げ上げ
	ThrowUp,
	//自由落下
	FreeFall,
	//上昇
	Rise,

};


/// <summary>
/// エミッタ
/// </summary>
struct Emitter {
	//エミッタのTransform;
	Transform transform;
	//発生数
	uint32_t count;
	//発生頻度
	float frequency;
	//頻度用時刻
	float frequencyTime;
};

/// <summary>
/// パーティクル(3D)
/// </summary>
class Particle3D {
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Particle3D();

	/// <summary>
	/// 生成
	/// </summary>
	/// <param name="moveType"></param>
	/// <returns></returns>
	static Particle3D* Create(const uint32_t& moveType);

	/// <summary>
	/// 生成
	/// </summary>
	/// <param name="moveType"></param>
	/// <returns></returns>
	static Particle3D* Create(const uint32_t& modelHandle,const uint32_t& moveType);


private:

#pragma region パーティクルの設定で使う関数

	/// <summary>
	/// 新しいパーティクルの生成
	/// </summary>
	/// <param name="randomEngine"></param>
	/// <returns></returns>
	Particle MakeNewParticle(std::mt19937& randomEngine);

	/// <summary>
	/// Emitterで発生させる
	/// </summary>
	/// <param name="emmitter"></param>
	/// <param name="randomEngine"></param>
	/// <returns></returns>
	std::list<Particle> Emission(const Emitter& emmitter, std::mt19937& randomEngine);



#pragma endregion

public:

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="camera"></param>
	void Update(const Camera& camera);

	/// <summary>
	/// 通常描画
	/// </summary>
	/// <param name="camera"></param>
	/// <param name="material"></param>
	void Draw(const Camera& camera, const Material& material);

	/// <summary>
	/// 描画(平行光源)
	/// </summary>
	/// <param name="camera"></param>
	/// <param name="material"></param>
	/// <param name="directionalLight"></param>
	void Draw(const Camera& camera,const Material& material,const DirectionalLight& directionalLight);

	/// <summary>
	/// 描画(点光源)
	/// </summary>
	/// <param name="camera"></param>
	/// <param name="material"></param>
	/// <param name="pointLight"></param>
	void Draw(const Camera& camera, const Material& material, const PointLight& pointLight);

	/// <summary>
	/// 描画(スポットライト)
	/// </summary>
	/// <param name="camera"></param>
	/// <param name="material"></param>
	/// <param name="spotLight"></param>
	void Draw(const Camera& camera, const Material& material, const SpotLight& spotLight);


	/// <summary>
	/// デストラクタ
	/// </summary>
	~Particle3D() = default;




public:
	/// <summary>
	///	一度だけ出すかどうか
	/// </summary>
	/// <param name="isReleaseOnce"></param>
	inline void SetIsReleaseOnceMode(const bool &isReleaseOnce) {
		this->isReleaseOnceMode_ = isReleaseOnce;
	}


	/// <summary>
	/// 透明になっていくようにするかどうか
	/// </summary>
	/// <param name="isToTransparent"></param>
	inline void SetIsToTransparent(const bool& isToTransparent) {
		this->isToTransparent_ = isToTransparent;
	}

	/// <summary>
	/// 全て透明になったかどうか
	/// </summary>
	/// <returns></returns>
	inline bool GetIsAllInvisible()const {
		return isAllInvisible_;
	}



#pragma region エミッタの中の設定


#pragma region SRT
	//Scale
	inline void SetScale(const Vector3& scale) {
		this->emitter_.transform.scale = scale;
	}

	//Rotate
	inline void SetRotate(const Vector3 &rotate) {
		this->emitter_.transform.rotate = rotate;
	}
	inline Vector3 GetRotate() const {
		return emitter_.transform.rotate;
	}

	//Translate
	inline void SetTranslate(const Vector3& translate) {
		this->emitter_.transform.translate = translate;
	}
	inline Vector3 GetTranslate() const{
		return emitter_.transform.translate;
	}

#pragma endregion

	//発生数
	inline void SetCount(const uint32_t& count) {
		this->emitter_.count = count;
	}
	//発生頻度
	inline void SetFrequency(const float& frequency) {
		this->emitter_.frequency = frequency;
	}
	//発生頻度を設定
	inline void SetFrequencyTime(const float& frequencyTime) {
		this->emitter_.frequencyTime = frequencyTime;
	}


#pragma endregion

private:

	//モデル管理クラス
	ModelManager* modelManager_ = nullptr;

	//テクスチャ管理クラス
	TextureManager* textureManager_ = nullptr;

	//DirectXクラス
	DirectXSetup* directXSetup_ = nullptr;

	//SRV管理クラス
	SrvManager* srvManager_ = nullptr;

	//パイプライン管理クラス
	PipelineManager* pipelineManager_ = nullptr;

private:
	

	//頂点リソースを作る
	ComPtr<ID3D12Resource> vertexResource_ = nullptr;
	//頂点バッファビューを作成する
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};
	//頂点データ
	std::vector<VertexData> vertices_{};


	//インスタンス
	ComPtr<ID3D12Resource>instancingResource_ = nullptr;

	//最大数
	const uint32_t MAX_INSTANCE_NUMBER_ = 100u;
	//描画すべきインスタンス数
	uint32_t numInstance_ = 0u;
	//インスタンスのインデックス
	int instancingIndex_ = 0;

	//一度だけ出すかどうか
	bool isReleaseOnceMode_ = false;
	//出し終えたかどうか
	bool isReeasedOnce_ = false;

	//パーティクル
	std::list<Particle>particles_;
	//パーティクルデータ
	ParticleForGPU* instancingData_ = nullptr;


	//テクスチャハンドル
	uint32_t textureHandle_ = 0;
	//動きの種類
	uint32_t moveType_ = NormalRelease;

	//透明になっていくか
	bool isToTransparent_ = true;
	//全て透明になったかどうか
	bool isAllInvisible_ = false;

	//エミッタの設定
	Emitter emitter_ = {};
	const float DELTA_TIME = 1.0f / 60.0f;


	//鉛直投げ上げ
	float velocityY_ = 1.2f;



	//カメラ
	//リソース
	ComPtr<ID3D12Resource>cameraResource_ = nullptr;
	//カメラデータ
	Vector3* cameraPositionData_ = {};
	//座標
	Vector3 cameraPosition_ = {};
};
