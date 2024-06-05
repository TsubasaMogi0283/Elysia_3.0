#pragma once
#include "DirectXSetup.h"
#include "Vector4.h"
#include "Vector3.h"
#include "VertexData.h"


class LuminanceBasedOutline{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	LuminanceBasedOutline()=default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 描画前の処理
	/// </summary>
	void PreDraw();
	void PreDrawSecond();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	
	/// <summary>
	/// デストラクタ
	/// </summary>
	~LuminanceBasedOutline()=default;

private:

	uint32_t textureHandle_ = 0;

	D3D12_RESOURCE_BARRIER barrier = {};
};

