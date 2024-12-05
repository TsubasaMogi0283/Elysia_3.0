#pragma once
/**
 * @file WinScene.h
 * @brief 勝利シーン
 * @author 茂木翼
 */
#include <memory>

#include "IGameScene.h"
#include "Sprite.h"
#include "Transform.h"
#include "Particle3D.h"
#include "Audio.h"
#include "Model.h"
#include "Input.h"


#pragma region 前方宣言

/// <summary>
/// ゲーム管理クラス
/// </summary>
class GameManager;

/// <summary>
/// 入力
/// </summary>
class Input;


#pragma endregion

/// <summary>
/// 勝利シーン
/// </summary>
class WinScene : public IGameScene {
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	WinScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize()override;

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="gameManager"></param>
	void Update(GameManager* gameManager)override;

	/// <summary>
	/// ポストエフェクト描画処理前
	/// </summary>
	void PreDrawPostEffectFirst()override;

	/// <summary>
	/// 描画(3D描画)
	/// </summary>
	void DrawObject3D()override;

	/// <summary>
	/// ポストエフェクト描画
	/// </summary>
	void DrawPostEffect()override;

	/// <summary>
	/// スプライトの描画
	/// </summary>
	void DrawSprite()override;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~WinScene();


private:
	//入力
	Input* input_ = nullptr;


private:
	//脱出成功のスプライト
	std::unique_ptr<Sprite> succeeded_ = nullptr;
	//のスプライト
	std::unique_ptr<Sprite> text_ = nullptr;
	//背景のスプライト
	std::unique_ptr<Sprite> black_ = nullptr;
	//透明度
	float transparency_ = 0.0f;
	
	//暗転している時間
	uint32_t blackOutTime_ = 0u;

	//Bトリガー
	uint32_t bTriggerTime_ = 0u;
	bool isBTrigger_ = false;

	//再開
	bool restart_ = false;




	//点滅
	bool isFlash_ = false;
	//時間
	uint32_t flashTime_ = 0u;
	const uint32_t FLASH_TIME_LIMIT_ = 30u;
	
	
	//高速点滅
	bool isFastFlash_ = false;
	const uint32_t FAST_FLASH_TIME_LIMIT_ = 60u;
	//間隔
	const uint32_t FAST_FLASH_TIME_INTERVAL_ = 3u;
	//見せるカウント
	uint32_t textDisplayCount_ = 0u;
	//時間
	uint32_t fastFlashTime_ = 0u;

};

