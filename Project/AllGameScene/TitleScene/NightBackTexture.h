#pragma once

/**
 * @file NightBackTexture.h
 * @brief 夜の背景クラス
 * @author 茂木翼
 */

#include "BaseTitleBackTexture.h"


/// <summary>
/// 夜の背景
/// </summary>
class NightBackTexture : public BaseTitleBackTexture{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	NightBackTexture() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize()override;

private:

};

