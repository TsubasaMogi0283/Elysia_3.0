#pragma once
/**
 * @file GameManager.h
 * @brief ゲーム管理クラス
 * @author 茂木翼
 */

#include <cstdint>
#include <memory>

#include "IGameScene.h"
#include "AbstractSceneFactory.h"

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
	/// シーン遷移
	/// </summary>
	/// <param name="sceneName"></param>
	void ChangeScene(const std::string& sceneName);

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

public:
	/// <summary>
	/// シーンファクトリーの設定
	/// </summary>
	/// <param name="abstractSceneFactory">シーンファクトリー</param>
	void SetAbstractSceneFactory(std::unique_ptr<IAbstractSceneFactory> abstractSceneFactory) {
		this->abstractSceneFactory_ = std::move(abstractSceneFactory);
	}


private:
	//シーンファクトリー
	std::unique_ptr<IAbstractSceneFactory> abstractSceneFactory_ = nullptr;

	//現在入っているシーン
	std::unique_ptr<IGameScene> currentGamaScene_ = nullptr;
	std::string  currentSceneName_ = "";
	
};