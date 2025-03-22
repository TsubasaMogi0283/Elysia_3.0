#pragma once

/**
 * @file TitleRailCamera.h
 * @brief レールカメラ(タイトル用)のクラス
 * @author 茂木翼
 */

#include <vector>
#include <sstream>

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
	/// <param name="csvPath"></param>
	void Initialize(const std::string& csvPath);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~TitleRailCamera() = default;


private:
	/// <summary>
	/// ImGui表示用関数
	/// </summary>
	void DisplayImGui();

	/// <summary>
	/// スプライン曲線を使った
	/// </summary>
	/// <param name="points"></param>
	/// <param name="t"></param>
	/// <returns></returns>
	Vector3 CatmullRomPositionLoop(const std::vector<Vector3>& points, const float& t);


public:
	/// <summary>
	/// カメラの取得
	/// </summary>
	/// <returns></returns>
	inline Camera GetCamera() const{
		return camera_;
	};

	/// <summary>
	/// 方向の取得
	/// </summary>
	/// <returns></returns>
	inline Vector3 GetDirection()const {
		return direction_;
	}


private:
	//ワールドトランスフォーム
	WorldTransform worldTransform_ = {};
	//カメラ
	Camera camera_ = {};

	//方向
	Vector3 direction_ = {};

	//線形補間用
	float cameraT_ = 0.0f;
	//tの増える値
	const float increaseTValue_ = 0.0001f;

	//制御点
	std::vector<Vector3>points_ = {};
	//生成の文字列を入れる
	std::stringstream positionsFromCSV_;


};

