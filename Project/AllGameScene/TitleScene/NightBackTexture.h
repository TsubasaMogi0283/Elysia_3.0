#pragma once
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

