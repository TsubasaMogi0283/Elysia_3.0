#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include <memory>
#include "Stage/Ground/StageRect.h"

struct Camera;
struct SpotLight;
struct Material;
class SampleScene;
class ObjectManager;

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
	inline void SetPointOfView(uint32_t pointOfView){
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
	inline float GetRadius() const {
		return radius_;
	}

	/// <summary>
	/// 持っている鍵の数を増やす
	/// </summary>
	inline void AddHaveKeyQuantity() {
		haveKeyQuantity_++;
	}


	/// <summary>
	/// 今鍵を何個持っているか
	/// </summary>
	/// <returns></returns>
	inline uint32_t GetHavingKey() {
		return haveKeyQuantity_;
	}

	/// <summary>
	/// 動く方向の設定
	/// </summary>
	/// <param name="move"></param>
	inline void SetMoveDirection(Vector3 moveDirection) {
		this->moveDirection_ = moveDirection;
	}

	inline void SetObjectManager(ObjectManager* objectManager) {
		this->objectManager_ = objectManager;
	}
	

	/// <summary>
	/// 四隅の取得
	/// </summary>
	/// <param name="stageRect"></param>
	/// <returns></returns>
	inline void SetStageRect(StageRect stageRect) {
		this->stageRect_ = stageRect;
	}

	/// <summary>
	/// 操作を受け付けるか受け付けないかの設定
	/// </summary>
	/// <param name="isControll"></param>
	inline void SetIsAbleToControll(bool isControll) {
		this->isAbleToControll_ = isControll;
	}


private:
	ObjectManager* objectManager_ = nullptr;

	//モデル
	std::unique_ptr<Model> model_ = nullptr;

	//ワールドトランスフォーム
	WorldTransform worldTransform_{};

	StageRect stageRect_ = {};

	//カメラ視点
	uint32_t pointOfView_ = 0;

	//半径
	float radius_ = 0.0f;

	//持っている鍵の数
	//可算なのでQuantity
	uint32_t haveKeyQuantity_ = 0;

	//動く方向
	Vector3 moveDirection_ = {};
	bool isAbleToMove_ = true;

	
	bool isPressKey_ = false;

	//操作可能かどうか
	bool isAbleToControll_ = false;

};

