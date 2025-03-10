#pragma once

/**
 * @file Particle2D.h
 * @brief パーティクル(2D版)のクラス
 * @author 茂木翼
 */

#include <random>
#include <list>

#include "DirectXSetup.h"
#include "VertexData.h"
#include "Particle.h"
#include "Emitter.h"
#include "ParticleMoveType.h"
#include "Material.h"



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
/// EllysiaEngine(前方宣言)
/// </summary>
namespace Ellysia {
	/// <summary>
	/// DirectXクラス
	/// </summary>
	class DirectSetup;

	/// <summary>
	/// SRV管理クラス
	/// </summary>
	class SrvManager;

	/// <summary>
	/// パイプライン管理クラス
	/// </summary>
	class PipelineManager;

	/// <summary>
	/// テクスチャ管理クラス
	/// </summary>
	class TextureManager;


};

class Particle2D{
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Particle2D();

	/// <summary>
	/// 生成
	/// </summary>
	/// <param name="moveType"></param>
	/// <returns></returns>
	static Particle2D* Create(const uint32_t& moveType);

private:

	/// <summary>
	/// 新しいパーティクルの生成
	/// </summary>
	/// <param name="randomEngine"></param>
	/// <returns></returns>
	ParticleInformation MakeNewParticle(std::mt19937& randomEngine);

	/// <summary>
	/// Emitterで発生させる
	/// </summary>
	/// <param name="emmitter"></param>
	/// <param name="randomEngine"></param>
	/// <returns></returns>
	std::list<ParticleInformation> Emission(const Emitter& emmitter, std::mt19937& randomEngine);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

public:

	/// <summary>
	/// 通常描画
	/// </summary>
	void Draw();


	/// <summary>
	/// デストラクタ
	/// </summary>
	~Particle2D() = default;




public:
	/// <summary>
	///	一度だけ出すかどうか
	/// </summary>
	/// <param name="isReleaseOnce"></param>
	inline void SetIsReleaseOnceMode(const bool& isReleaseOnce) {
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
	/// <returns>全て消えたことを示すフラグ</returns>
	inline bool GetIsAllInvisible()const {
		return isAllInvisible_;
	}

	/// <summary>
	/// 地面のオフセット
	/// </summary>
	/// <param name="offset">オフセット</param>
	inline void SetGroundOffset(const float& offset) {
		this->groundOffset_ = offset;
	}

#pragma region エミッタの中の設定


	/// <summary>
	/// スケールの設定
	/// </summary>
	/// <param name="scale">スケールサイズ</param>
	inline void SetScale(const Vector3& scale) {
		this->emitter_.transform.scale = scale;
	}

	/// <summary>
	/// 回転の設定
	/// </summary>
	/// <param name="rotate">回転</param>
	inline void SetRotate(const Vector3& rotate) {
		this->emitter_.transform.rotate = rotate;
	}
	/// <summary>
	/// 回転の取得
	/// </summary>
	/// <returns></returns>
	inline Vector3 GetRotate() const {
		return emitter_.transform.rotate;
	}

	/// <summary>
	/// 座標の設定
	/// </summary>
	/// <param name="translate">座標</param>
	inline void SetTranslate(const Vector3& translate) {
		this->emitter_.transform.translate = translate;
	}
	/// <summary>
	/// 座標の取得
	/// </summary>
	/// <returns></returns>
	inline Vector3 GetTranslate() const {
		return emitter_.transform.translate;
	}


	/// <summary>
	/// 発生数
	/// </summary>
	/// <param name="count"></param>
	inline void SetCount(const uint32_t& count) {
		this->emitter_.count = count;
	}

	/// <summary>
	/// 発生頻度
	/// </summary>
	/// <param name="frequency"></param>
	inline void SetFrequency(const float_t& frequency) {
		this->emitter_.frequency = frequency;
	}

	/// <summary>
	/// 発生頻度を設定
	/// </summary>
	/// <param name="frequencyTime"></param>
	inline void SetFrequencyTime(const float_t& frequencyTime) {
		this->emitter_.frequencyTime = frequencyTime;
	}


#pragma endregion

private:

	//テクスチャ管理クラス
	Ellysia::TextureManager* textureManager_ = nullptr;
	//DirectXクラス
	Ellysia::DirectXSetup* directXSetup_ = nullptr;
	//SRV管理クラス
	Ellysia::SrvManager* srvManager_ = nullptr;
	//パイプライン管理クラス
	Ellysia::PipelineManager* pipelineManager_ = nullptr;

private:

	/// <summary>
	/// 頂点の位置
	/// </summary>
	enum VERTEX_POSITION {
		//左下
		LeftBottom,
		//左上
		LeftTop,
		//右下
		RightBottom,
		//右上
		RightTop,

	};


	//頂点リソースを作る
	ComPtr<ID3D12Resource> vertexResource_ = nullptr;
	//頂点バッファビューを作成する
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};


	//インスタンス
	ComPtr<ID3D12Resource>instancingResource_ = nullptr;

	//マテリアル
	//リソース
	ComPtr<ID3D12Resource> materialResource_ = nullptr;
	//データ
	MaterialData* materialData_ = nullptr;
	//色
	Vector4 color_ = {.x=1.0f,.y=1.0f, .z=1.0f,.w=1.0f};

	//UVトランスフォーム
	Transform uvTransform_ = {};



	//最大数
	const uint32_t MAX_INSTANCE_NUMBER_ = 100u;
	//描画すべきインスタンス数
	uint32_t numInstance_ = 0u;
	//インスタンスのインデックス
	int instancingIndex_ = 0;

	//一度だけ出すかどうか
	bool isReleaseOnceMode_ = true;
	//出し終えたかどうか
	bool isReeasedOnce_ = false;

	//パーティクル
	std::list<ParticleInformation>particles_;
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
	const float_t DELTA_TIME = 1.0f / 60.0f;


	//鉛直投げ上げ
	float_t velocityY_ = 1.2f;
	//地面の高さ設定
	float_t groundOffset_ = 0.0f;


};

