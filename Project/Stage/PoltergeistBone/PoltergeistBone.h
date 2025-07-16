#pragma once

/**
 * @file GameManager.h
 * @brief ドア(レベルエディタ)の処理用クラス
 * @author 茂木翼
 */

#include <memory>

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

	/// <summary>
	/// オーディオ
	/// </summary>
	class Audio;

}

/// <summary>
/// プレイヤー
/// </summary>
class Player;

/// <summary>
/// ドア(レベルエディタ)
/// </summary>
class PoltergeistBone {
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	PoltergeistBone();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 3Dモデルの描画
	/// </summary>
	void DrawObject3D();


	/// <summary>
	/// スプライトの描画
	/// </summary>
	void DrawSprite();

	
	/// <summary>
	/// デストラクタ
	/// </summary>
	~PoltergeistBone() = default;

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

};