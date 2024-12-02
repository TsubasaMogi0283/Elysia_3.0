#pragma once
#include <memory>
#include "Model.h"
#include "WorldTransform.h"
#include "Material.h"

struct Camera;

/// <summary>
/// 鍵
/// </summary>
class Key{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Key() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="modelhandle"></param>
	/// <param name="position"></param>
	void Initialize(uint32_t& modelhandle,Vector3& position);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	/// <param name="spotLight"></param>
	void Draw(Camera& camera,SpotLight& spotLight);


	/// <summary>
	/// デストラクタ
	/// </summary>
	~Key() = default;

public:
	/// <summary>
	/// 鍵がプレイヤーに取得される
	/// </summary>
	void PickedUp();

public:

	/// <summary>
	/// ワールド座標を取得する
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition()const;

	/// <summary>
	/// 半径の取得
	/// </summary>
	/// <returns></returns>
	float GetRadius()const {
		return radius_;
	}

	/// <summary>
	/// 取得したかどうか
	/// </summary>
	/// <returns></returns>
	bool GetIsPickUp()const {
		return isPickUp_;
	}

	/// <summary>
	/// 取得前
	/// </summary>
	/// <param name="isPrePickUp"></param>
	inline void SetIsPrePickUp(bool isPrePickUp) {
		this->isPrePickUp_ = isPrePickUp;
	}
	inline bool GetIsPrePickUp()const {
		return isPrePickUp_;
	}


private:
	//モデル
	std::unique_ptr<Model> model_ = nullptr;
	//ワールドトランスフォーム
	WorldTransform worldTransform_ = {};
	//マテリアル
	Material material_ = {};
	
	//取得する前
	bool isPrePickUp_ = false;
	//取得されたか
	bool isPickUp_ = false;

	//半径
	float radius_ = 0.0f;

};

