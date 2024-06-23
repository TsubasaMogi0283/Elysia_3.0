#pragma once
#include <memory>
#include "Model.h"
#include "WorldTransform.h"
#include "Material.h"

struct Camera;

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
	/// ワールド座標を取得する
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition()const;

	float GetRadius()const {
		return radius_;
	}

	bool GetIsPickUp()const {
		return isPickUp_;
	}

private:
	WorldTransform worldTransform_ = {};
	Material material_ = {};
	std::unique_ptr<Model> model_ = nullptr;;

	//取得されたか
	bool isPickUp_ = false;

	//半径
	float radius_ = 0.0f;

};

