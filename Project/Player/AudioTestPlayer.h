#pragma once
#include "WorldTransform.h"
#include "Material.h"
#include "Model.h"
#include "PlayerCollisionToAudioObject.h"

struct DirectionalLight;
struct Camera;

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
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	/// <param name="directionalLight"></param>
	void Draw(const Camera& camera, const DirectionalLight& directionalLight);


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

	/// <summary>
	/// コライダー取得
	/// </summary>
	/// <returns></returns>
	PlayerCollisionToAudioObject* GetCollosion() const{
		return collosion_.get();;
	}



private:
	//ワールドトランスフォーム
	WorldTransform worldTransform_ = {};

	//マテリアル
	Material material_ = {};

	//モデル
	std::unique_ptr<Model>model_ = nullptr;


	//方向
	Vector3 direction_ = {};

	//コライダー
	std::unique_ptr<PlayerCollisionToAudioObject>collosion_ = nullptr;

};