#pragma once
#include "DirectXSetup.h"
#include "Vector4.h"
#include "Vector3.h"
#include "VertexData.h"


class DepthBasedOutline{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	DepthBasedOutline() = default;

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
	~DepthBasedOutline() = default;

private:

	uint32_t textureHandle_ = 0;

	D3D12_RESOURCE_BARRIER barrier = {};





};

