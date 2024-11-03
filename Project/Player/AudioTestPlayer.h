#pragma once
#include "WorldTransform.h"
#include "Material.h"
#include "Camera.h"

struct SpotLight;

class AudioTestPlayer{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	AudioTestPlayer() = default;


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
	~AudioTestPlayer() = default;

public:
	/// <summary>
	/// 方向を設定
	/// </summary>
	/// <param name="direction"></param>
	void SetDirection(const Vector3& direction) {
		this->direction_ = direction;
	}

	
	/// <summary>
	/// ワールド座標を取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition()const {
		return worldTransform_.GetWorldPosition();
	}


	/// <summary>
	/// 方向を取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetDirection()const {
		return direction_;
	}

private:

	//ワールドトランスフォーム
	WorldTransform worldTransform_ = {};


	Vector3 direction_ = {};
};