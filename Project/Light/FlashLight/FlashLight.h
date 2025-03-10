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
	/// チャージ
	/// </summary>
	void Charge();

	/// <summary>
	/// ImGuiの表示
	/// </summary>
	void ImGuiDisplay();

	/// <summary>
	/// 調整
	/// </summary>
	void Adjustment();

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
	inline void SetTheta(const float_t& theta) {
		this->theta_ = theta;
	}

	/// <summary>
	/// φの設定
	/// </summary>
	/// <param name="phi">φ</param>
	inline void SetPhi(const float_t& phi) {
		this->phi_ = phi;
	}

	/// <summary>
	/// ライトの幅を設定
	/// </summary>
	/// <param name="lightSideTheta">幅の角度</param>
	inline void SetLightSideTheta(const float_t& lightSideTheta) {
		this ->lightSideTheta_ = lightSideTheta;
	}

	/// <summary>
	/// 方向を取得
	/// </summary>
	/// <returns>方向</returns>
	inline Vector3 GetDirection()const {
		return direction_;
	}

	/// <summary>
	/// 最大の幅
	/// </summary>
	/// <param name="range">幅</param>
	inline void SetMaxRange(const float_t& range) {
		this->maxRange_ = range;
	}


	/// <summary>
	/// 最小の幅
	/// </summary>
	/// <param name="range">幅</param>
	inline void SetMinRange(const float_t& range) {
		this->minRange_ = range;
	}

	/// <summary>
	/// チャージをしているかどうかの設定
	/// </summary>
	/// <param name="isCharge">チャージ中か</param>
	inline void SetIsCharge(const bool& isCharge) {
		this->isCharge_ = isCharge;
	}

	/// <summary>
	/// 攻撃可能かどうかを取得
	/// </summary>
	/// <returns>攻撃可能フラグ</returns>
	inline bool GetIsAbleToAttack() const {
		return isAbleToAttack_;
	}


	/// <summary>
	/// 扇の取得(3次元)
	/// </summary>
	/// <returns>扇</returns>
	inline Fan3D GetFan3D() {
		return fan3D_;
	}

	/// <summary>
	/// コリジョンの取得
	/// </summary>
	/// <returns>コリジョン</returns>
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
	//cosFallowoffStart
	const std::string FLASH_LIGHT_COS_FALLOWOFF_START_STRING_ = "FlashLightCosFallowOffStart";
	//最大
	const std::string MAX_STRING_ = "Max";
	float_t maxIntensity_ = 400.0f;
	float_t maxStart_ = 2.4f;
	//最小
	const std::string MIN_STRING_ = "Min";
	float_t minIntensity_ = 50.0f;
	float_t minStart_ = 1.5f;

	//チャージ
	std::string FLASH_LIGHT_CHARGE_VALUE_ = "FlashLightCharge";
	//増える値
	const std::string CHARGE_STRING_ = "IncreaseChargeValue";
	float_t chargeIncreaseValue_ = 0.1f;
	//攻撃可能になる値
	const std::string IS_ABLE_TO_CHARGE_VALUE_STRING = "IsAbleToChargeValue";
	float_t isAbleToAttackValue_ = 5.0f;

private:
	//チャージスピード
	const float_t CHARGE_INTERVAL_VALUE_ = 0.1f;
	//光の届く距離
	const float_t LIGHT_DISTANCE = 22.0f;

private:
	//ここに値を入れてゲームシーンで他のオブジェクトに適用させる
	SpotLight spotLight_ = {};

	//プレイヤーの座標
	Vector3 playerPosition_ = {};

	
	//座標
	Vector3 position_ = {};
	//方向
	Vector3 direction_ = {};
	//幅
	float_t lightSideTheta_ = 0.0f;

	//ライトの幅
	float_t maxRange_ = 0.0f;
	float_t minRange_ = 0.0f;
	float_t ratio = 0.0f;

	//角度
	float_t theta_ = 0.0f;
	float_t phi_ = 0.0f;

	//扇
	Fan3D fan3D_ = {};


	//チャージしているかどうか
	bool isCharge_ = false;
	//チャージの値
	float_t chargeValue_ = 0.0f;
	//攻撃できるかどうか
	bool isAbleToAttack_ = false;

private:
	//当たり判定
	std::unique_ptr<FlashLightCollision> flashLightCollision_ = nullptr;

private:

#ifdef _DEBUG
	
	//デバッグ用
	//左右
	const uint32_t RIGHT_ = 0u;
	const uint32_t LEFT_ = 1u;

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

