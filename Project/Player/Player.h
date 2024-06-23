#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include <memory>

struct Camera;
struct SpotLight;
struct Material;
class SampleScene;


enum PlayerViewOfPoint {
	//未定
	NonePerson = 0,
	//1人称視点
	FirstPerson = 1,
	//3人称後方
	ThirdPersonBack = 2,
	//3人称後方
	ThirdPersonFront = 3,

};

class Player{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Player() = default;

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
	/// <param name="material"></param>
	/// <param name="spotLight"></param>
	void Draw(Camera& camera,Material& material,SpotLight& spotLight);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player() = default;

public:
	/// <summary>
	/// 1人称または3人称かの設定
	/// 1人称だったらモデルは表示させない
	/// </summary>
	/// <param name="pointOfView"></param>
	void SetPointOfView(uint32_t pointOfView){
		this->pointOfView_ = pointOfView;
	}

	//Getterは後ろにconst を付ける方が良いらしい
	//オブジェクトの状態を変更しない読み取り専用だから


	/// <summary>
	/// ワールド座標を所得
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition()const;

	/// <summary>
	/// 半径を取得
	/// </summary>
	/// <returns></returns>
	float GetRadius() const {
		return radius_;
	}

	/// <summary>
	/// 持っている鍵の数を増やす
	/// </summary>
	void AddHaveKeyQuantity();



	
public:
	







private:
	std::unique_ptr<Model> model_ = nullptr;

	WorldTransform worldTransform_{};

	//カメラ視点
	uint32_t pointOfView_ = 0;


	float radius_ = 0.0f;

	//持っている鍵の数
	//可算なのでQuantity
	uint32_t haveKeyQuantity_ = 0;


};

