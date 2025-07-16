#pragma once

/**
 * @file GameManager.h
 * @brief ドア(レベルエディタ)の処理用クラス
 * @author 茂木翼
 */

#include <memory>

#include <Sprite.h>


 /// <summary>
 /// ElysiaEngine
 /// </summary>
namespace Elysia {
	/// <summary>
	/// 入力
	/// </summary>
	class Input;

	/// <summary>
	/// レベル管理クラス
	/// </summary>
	class LevelDataManager;
}

/// <summary>
/// プレイヤー
/// </summary>
class Player;

/// <summary>
/// ドア(レベルエディタ)
/// </summary>
class Door {
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Door();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// スプライトの描画
	/// </summary>
	void DrawSprite();


	/// <summary>
	/// デストラクタ
	/// </summary>
	~Door() = default;

public:
	/// <summary>
	/// レベルデータのハンドルを取得
	/// </summary>
	/// <param name="levelDataHandle">ハンドル</param>
	inline void SetLevelDataHandle(const uint32_t& levelDataHandle) {
		this->levelDataHandle_ = levelDataHandle;
	}
	/// <summary>
	/// プレイヤーの設定
	/// </summary>
	/// <param name="player">プレイヤー</param>
	inline void SetPlayer(Player* player) {
		this->player_ = player;
	}


private:
	
	//入力クラス
	Elysia::Input* input_ = nullptr;
	//レベルエディタ
	Elysia::LevelDataManager* levelDataManager_ = nullptr;
	//ハンドル
	uint32_t levelDataHandle_ = 0u;
	//プレイヤー
	Player* player_ = nullptr;

private:
	//線形補間の最小値
	const float_t MIN_T_VALUE_ = 0.0f;
	//線形補間の最大値
	const float_t MAX_T_VALUE_ = 1.0f;
	//回転
	const float_t ROTATE_VALUE_ = 0.01f;
private:
	//開ける
	std::unique_ptr<Elysia::Sprite>openSptrite_ = nullptr;

	//初期回転
	float_t initialRotateY_ = {};
	//最大回転
	float_t maxDoorRotateY_ = {};
	float_t doorRotateY_ = {};
	bool isDoorOpen_ = false;
	float_t rotateT_ = 0.0f;
	float_t doorLength = 0.0f;
};