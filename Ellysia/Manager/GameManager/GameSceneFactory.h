#pragma once

/**
 * @file GameSceneFactory.h
 * @brief ゲームシーンファクトリー
 * @author 茂木翼
 */

#include <string>

#include "IGameScene.h"

/// <summary>
/// ゲームシーンファクトリー
/// </summary>
class GameSceneFactory{
public:
	/// <summary>
	/// シーンの生成
	/// </summary>
	/// <param name="sceneName">シーンの名前</param>
	/// <returns>生成したシーン</returns>
	IGameScene* CreateScene(const std::string& sceneName);

};

