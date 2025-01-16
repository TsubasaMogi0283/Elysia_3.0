#pragma once
#include "Collider.h"

#pragma region 前方宣言
/// <summary>
/// カメラ
/// </summary>
struct Camera;

/// <summary>
/// スポットライト
/// </summary>
struct SpotLight;


#pragma endregion

/**
 * @file BasedPlayerCollision.h
 * @brief プレイヤー用のコリジョンクラス
 * @author 茂木翼
 */

/// <summary>
/// プレイヤー用のコリジョン
/// </summary>
class BasedPlayerCollision: public Collider{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	BasedPlayerCollision() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="modelHandle"></param>
	virtual void Initialize()=0;

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update()=0;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	/// <param name="material"></param>
	/// <param name="spotLight"></param>
	virtual void Draw(const Camera& camera, const SpotLight& spotLight)=0;

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~BasedPlayerCollision() = default;


public:
	/// <summary>
	/// プレイヤーの座標の設定
	/// </summary>
	/// <param name="position"></param>
	virtual inline void SetPlayerPosition(const Vector3& position) {
		this->position_ = position;
	}

	/// <summary>
	/// 衝突
	/// </summary>
	/// <returns></returns>
	virtual inline bool GetIsTouch()const {
		return isTouch_;
	}



protected:
	//位置
	Vector3 position_ = {};


	//衝突
	bool isTouch_ = false;

};

