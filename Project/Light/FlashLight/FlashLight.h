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


private:
	//ここに値を入れてゲームシーンで他のオブジェクトに適用させる
	SpotLight spotLight_ = {};

	//プレイヤーの座標
	Vector3 playerPosition_ = {};

};

