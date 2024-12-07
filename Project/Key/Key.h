#pragma once
/**
 * @file Key.h
 * @brief 鍵のクラス
 * @author 茂木翼
 */

#include <memory>

#include "Model.h"
#include "WorldTransform.h"
#include "Material.h"

#pragma region 前方宣言

/// <summary>
/// カメラ
/// </summary>
struct Camera;

#pragma endregion


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
	void Initialize(const uint32_t& modelhandle,const Vector3& position);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	/// <param name="spotLight"></param>
	void Draw(const Camera& camera,const SpotLight& spotLight);


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
	Vector3 GetWorldPosition()const {
		return worldTransform_.GetWorldPosition();
	}

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

	/// <summary>
	/// 取得可能かどうか
	/// </summary>
	/// <returns></returns>
	inline bool GetIsPrePickUp()const {
		return isPrePickUp_;
	}



private:
	//回転の大きさ
	const float ROTATE_AMOUNT_ = 0.1f;
	//上下移動の大きさ
	const float MOVE_AMOUNT_ = 0.1f;



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

	//元の座標
	float originalPositionY_ = 0.001f;

	//sinの動きにしたいのでthetaを作る
	float theta_ = 0.0f;
};

