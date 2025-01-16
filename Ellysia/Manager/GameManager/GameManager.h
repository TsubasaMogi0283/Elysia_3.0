#pragma once
/**
 * @file GameManager.h
 * @brief ゲーム管理クラス
 * @author 茂木翼
 */

#include <cstdint>
#include <memory>

#include "IGameScene.h"


/// <summary>
/// ゲーム管理クラス
/// </summary>
class GameManager {
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	GameManager() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// シーンチェンジ
	/// </summary>
	/// <param name="newGameScene">新しいシーン</param>
	void ChangeScene(std::unique_ptr<IGameScene> newGameScene);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画(3Dオブジェクト)
	/// </summary>
	void DrawObject3D();

	/// <summary>
	/// 描画(スプライト)
	/// </summary>
	void DrawSprite();

	/// <summary>
	/// ポストエフェクト描画前処理
	/// </summary>
	void PreDrawPostEffectFirst();

	/// <summary>
	/// 描画(ポストエフェクト)
	/// </summary>
	void DrawPostEffect();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameManager() = default;

private:

	//現在入っているシーン
	std::unique_ptr<IGameScene> currentGamaScene_ = nullptr;

};