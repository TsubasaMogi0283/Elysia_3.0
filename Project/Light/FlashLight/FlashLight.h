#pragma once
#include <memory>

#include "SpotLight.h"
#include <Fan.h>
#include <WorldTransform.h>
#include <Model.h>
#include "Material.h"

struct Camera;

class FlashLight{
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
	void Draw(Camera& camera);

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
	inline void SetPlayerPosition(Vector3 position) {
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
	/// 扇を取得
	/// </summary>
	/// <returns></returns>
	inline Fan GetFan() const{
		return fan_;
	}

private:
	//ここに値を入れてゲームシーンで他のオブジェクトに適用させる
	SpotLight spotLight_ = {};

	//プレイヤーの座標
	Vector3 playerPosition_ = {};

	Vector3 lightPosition = {};
	Vector3 lightDirection_ = {};
	float degree_ = 0.0f;

	//角度
	float theta_ = 0.0f;
	float phi_ = 0.0f;

	//扇
	Fan fan_ = {};


	enum Side {
		Right,
		Left,
	};
	static const uint32_t SIDE_QUANTITY_ = 2;
	std::unique_ptr<Model>model_[SIDE_QUANTITY_] = { nullptr };
	WorldTransform worldTransform_[SIDE_QUANTITY_] = {};
	Material material_ = {};
};

