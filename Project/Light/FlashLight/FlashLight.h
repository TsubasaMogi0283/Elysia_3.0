#pragma once
#include "SpotLight.h"

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


private:
	//ここに値を入れてゲームシーンで他のオブジェクトに適用させる
	SpotLight spotLight_ = {};

	//プレイヤーの座標
	Vector3 playerPosition_ = {};

	Vector3 lightPosition = {};
	Vector3 lightDirection_ = {};


	//角度
	float theta_ = 0.0f;
	float phi_ = 0.0f;

};

