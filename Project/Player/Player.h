#pragma once
#include "WorldTransform.h"
#include "Model.h"

struct Camera;
class SampleScene;

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
	void Draw(Camera& camera);

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
	void SetPointOfView(uint32_t pointOfView) {
		this->pointOfView_ = pointOfView;
	}




private:
	std::unique_ptr<Model> model_ = nullptr;

	WorldTransform worldTransform_{};

	//カメラ視点
	uint32_t pointOfView_ = 0;




};

