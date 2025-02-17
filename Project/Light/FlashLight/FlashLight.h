#pragma once
/**
 * @file FlashLight.h
 * @brief 懐中電灯クラス
 * @author 茂木翼
 */

#include <memory>

#include "SpotLight.h"
#include "Fan.h"
#include "WorldTransform.h"
#include "Model.h"
#include "Material.h"
#include "FlashLightCollision.h"


 /// <summary>
 /// EllysiaEngine(前方宣言)
 /// </summary>
namespace Ellysia {
	/// <summary>
	/// グローバル変数
	/// </summary>
	class GlobalVariables;
}

/// <summary>
/// カメラ
/// </summary>
struct Camera;

/// <summary>
/// 懐中電灯
/// </summary>
class FlashLight {
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	FlashLight();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画(デバッグ用)
	/// </summary>
	/// <param name="camera">カメラ</param>
	void Draw(const Camera& camera);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~FlashLight()=default;

private:
	/// <summary>
	/// IMGuiの表示
	/// </summary>
	void ImGuiDisplay();

public:
	/// <summary>
	/// スポットライトの取得
	/// </summary>
	/// <returns>スポットライト</returns>
	inline SpotLight GetSpotLight() const{
		return spotLight_;
	}


	/// <summary>
	/// プレイヤーの座標を取得
	/// </summary>
	/// <param name="position">座標</param>
	inline void SetPlayerPosition(const Vector3& position) {
		this->playerPosition_ = position;
	}

	/// <summary>
	/// θの設定
	/// </summary>
	/// <param name="theta">θ</param>
	inline void SetTheta(const float& theta) {
		this->theta_ = theta;
	}

	/// <summary>
	/// φの設定
	/// </summary>
	/// <param name="phi">φ</param>
	inline void SetPhi(const float& phi) {
		this->phi_ = phi;
	}

	/// <summary>
	/// ライトの幅を設定
	/// </summary>
	/// <param name="lightSideTheta"></param>
	inline void SetLightSideTheta(const float& lightSideTheta) {
		this ->lightSideTheta_ = lightSideTheta;
	}

	/// <summary>
	/// 方向を取得
	/// </summary>
	/// <returns></returns>
	inline Vector3 GetDirection()const {
		return direction_;
	}

	/// <summary>
	/// 最大の幅
	/// </summary>
	/// <param name="range"></param>
	inline void SetMaxRange(const float& range) {
		this->maxRange_ = range;
	}


	/// <summary>
	/// 最小の幅
	/// </summary>
	/// <param name="range"></param>
	inline void SetMinRange(const float& range) {
		this->minRange_ = range;
	}

	/// <summary>
	/// 扇の取得(3次元)
	/// </summary>
	/// <returns></returns>
	inline Fan3D GetFan3D() {
		return fan3D_;
	}

	/// <summary>
	/// 当たり判定の取得
	/// </summary>
	/// <returns></returns>
	inline FlashLightCollision* GetFanCollision()const {
		return flashLightCollision_.get();
	}


private:
	//グローバル変数クラス
	Ellysia::GlobalVariables* globalVariables_ = nullptr;

private:
	//調整項目
	//光の強さ
	const std::string FLASH_LIGHT_INTENSITY_STRING_ = "FlashLightIntensity";

	//ライトの最大の強さ
	const std::string MAX_INTENSITY_STRING_ = "Max";
	float maxIntensity_ = 400.0f;
	//ライトの最小の強さ
	const std::string MIN_INTENSITY_STRING_ = "Min";
	float minIntensity_ = 50.0f;
private:
	//ここに値を入れてゲームシーンで他のオブジェクトに適用させる
	SpotLight spotLight_ = {};

	//プレイヤーの座標
	Vector3 playerPosition_ = {};

	//光の届く距離
	const float LIGHT_DISTANCE = 22.0f;
	//座標
	Vector3 position_ = {};
	//方向
	Vector3 direction_ = {};
	//幅
	float lightSideTheta_ = 0.0f;

	//ライトの幅
	float maxRange_ = 0.0f;
	float minRange_ = 0.0f;
	float ratio = 0.0f;

	//角度
	float theta_ = 0.0f;
	float phi_ = 0.0f;

	//扇
	Fan3D fan3D_ = {};


	//当たり判定
	std::unique_ptr<FlashLightCollision> flashLightCollision_ = nullptr;

#ifdef _DEBUG
	
	//デバッグ用
	//左右
	const uint32_t Right = 0u;
	const uint32_t Left = 1u;

	static const uint32_t SIDE_QUANTITY_ = 2u;
	std::unique_ptr<Ellysia::Model>model_[SIDE_QUANTITY_] = { nullptr };
	WorldTransform worldTransform_[SIDE_QUANTITY_] = {};
	Material material_ = {};

	//中心
	std::unique_ptr<Ellysia::Model>lightCenterModel_ = nullptr;
	WorldTransform lightCenterWorldTransform_ = {};
	Material lightCenterMaterial_ = {};

#endif // _DEBUG
	

};

