#pragma once

/**
 * @file TitleRailCamera.h
 * @brief レールカメラ(タイトル用)のクラス
 * @author 茂木翼
 */

#include <vector>

#include "WorldTransform.h"
#include "Camera.h"

/// <summary>
/// レールカメラ(タイトル用)
/// </summary>
class TitleRailCamera{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	TitleRailCamera() = default;


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
	~TitleRailCamera() = default;


public:
	WorldTransform GetWorldTransform() const{
		return worldTransform_;
	};

	Camera GetCamera() const{
		return camera_;
	};


private:
	/// <summary>
	/// スプライン曲線を使った
	/// </summary>
	/// <param name="points"></param>
	/// <param name="t"></param>
	/// <returns></returns>
	Vector3 CatmullRomPositionLoop(const std::vector<Vector3>& points, float t);


private:
	//ワールドトランスフォーム
	WorldTransform worldTransform_ = {};


	//カメラ
	Camera camera_ = {};

	//線形補間
	float cameraT_ = 0.0f;

	
	std::vector<Vector3>points_;


};

