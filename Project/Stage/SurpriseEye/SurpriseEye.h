#pragma once

/**
 * @file SurpriseEye.h
 * @brief びっくり要素のクラス
 * @author 茂木翼
 */


#include <Sprite.h>
#include <memory>

/// <summary>
/// ElysiaEngine
/// </summary>
namespace Elysia {
	/// <summary>
	/// テクスチャ管理クラス
	/// </summary>
	class TextureManager;

	/// <summary>
	/// オーディオ
	/// </summary>
	class Audio;
}

/// <summary>
/// びっくり要素
/// </summary>
class SurpriseEye{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	SurpriseEye();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void DrawSprite();
	
	/// <summary>
	/// デストラクタ
	/// </summary>
	~SurpriseEye()=default;

public:
	/// <summary>
	/// 非表示の設定
	/// </summary>
	inline void SetIsInvisible() {
		this->isInvisible_=false;
	}

	/// <summary>
	/// 非表示を取得
	/// </summary>
	/// <returns></returns>
	inline bool GetIsInvisible()const {
		return isInvisible_;
	}

private:
	//テクスチャ管理クラス
	Elysia::TextureManager* textureManager_ = nullptr;
	//オーディオ
	Elysia::Audio* audio_ = nullptr;

private:
	//時間変化
	const float_t DELTA_TIME_ = 1.0f / 60.0f;
	//最大表示時間
	const float_t MAX_DISPLAY_TIME_ = 2.0f;

private:
	//目のスプライト
	std::unique_ptr<Elysia::Sprite>eye_ = nullptr;
	//びっくりSE
	uint32_t surpriseSeHandle_ = 0u;

	//非表示
	bool isInvisible_ = true;
	//表示の時間
	float_t displayTime_ = 0.0f;
};

