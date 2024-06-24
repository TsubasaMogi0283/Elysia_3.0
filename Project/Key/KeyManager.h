#pragma once
#include <memory>
#include <list>

#include "Key.h"

class KeyManager{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	KeyManager() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="modelHandle"></param>
	void Initialize(uint32_t modelHandle);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~KeyManager() = default;


private:
	std::list<Key*>keyes_ = {};

};

