#pragma once
/**
 * @file FlashLight.h
 * @brief 懐中電灯クラス
 * @author 茂木翼
 */

#include <memory>

#include "SpotLight.h"
#include "Fan.h"
#include "WorldTransform.h"
#include "Model.h"
#include "Material.h"
#include "FlashLightCollision.h"

/// <summary>
/// カメラ
/// </summary>
struct Camera;

/// <summary>
/// 懐中電灯
/// </summary>
class FlashLight {
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	FlashLight() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画(デバッグ用)
	/// </summary>
	/// <param name="camera"></param>
	void Draw(const Camera& camera);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~FlashLight() = default;


public:
	/// <summary>
	/// スポットライトの取得
	/// </summary>
	/// <returns></returns>
	inline SpotLight GetSpotLight() const{
		return spotLight_;
	}


	/// <summary>
	/// プレイヤーの座標を取得
	/// </summary>
	/// <param name="position"></param>
	inline void SetPlayerPosition(const Vector3& position) {
		this->playerPosition_ = position;
	}

	inline void SetTheta(float theta) {
		this->theta_ = theta;
	}

	inline void SetPhi(float phi) {
		this->phi_ = phi;
	}

	/// <summary>
	/// 方向を取得
	/// </summary>
	/// <returns></returns>
	inline Vector3 GetDirection()const {
		return lightDirection_;
	}


	/// <summary>
	/// 扇の取得(3次元)
	/// </summary>
	/// <returns></returns>
	inline Fan3D GetFan3D() {
		return fan3D_;
	}


	/// <summary>
	/// 当たり判定の取得
	/// </summary>
	/// <returns></returns>
	inline FlashLightCollision* GetFanCollision()const {
		return flashLightCollision_.get();
	}


private:
	//ここに値を入れてゲームシーンで他のオブジェクトに適用させる
	SpotLight spotLight_ = {};

	//プレイヤーの座標
	Vector3 playerPosition_ = {};

	//光の届く距離
	const float LIGHT_DISTANCE = 22.0f;
	Vector3 lightPosition = {};
	Vector3 lightDirection_ = {};
	float lightSideTheta = 0.0f;

	//角度
	float theta_ = 0.0f;
	float phi_ = 0.0f;

	//扇
	Fan3D fan3D_ = {};

	//デバッグ用のモデル
	//左右
	enum Side {
		Right,
		Left,
	};
	static const uint32_t SIDE_QUANTITY_ = 2;
	std::unique_ptr<Ellysia::Model>model_[SIDE_QUANTITY_] = { nullptr };
	WorldTransform worldTransform_[SIDE_QUANTITY_] = {};
	Material material_ = {};

	//中心
	std::unique_ptr<Ellysia::Model>lightCenterModel_ = nullptr;
	WorldTransform lightCenterWorldTransform_ = {};
	Material lightCenterMaterial_ = {};

	//当たり判定
	std::unique_ptr<FlashLightCollision> flashLightCollision_ = nullptr;

};

