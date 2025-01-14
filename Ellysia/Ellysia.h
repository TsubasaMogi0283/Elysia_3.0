#pragma once

/**
 * @file Ellysia.h
 * @brief EllysiaEngine(メインフレーム)のクラス
 * @author 茂木翼
 */

#include "WindowsSetup.h"
#include "DirectXSetup.h"
#include "GameManager.h"

//Ellysia(エリシア・エレシア)
//Aile + Sheer...「エイル・エル(フランス語で自分の名前になる)」と「シア」
//sheer・・・全くの、完全な。混じりけのない、純粋な
//「Ellysia Engine」というように、後ろにEngine加えても自然な語感だと思う

//崩〇3〇dじゃないよ。
//Elysiaでもないよ。誤字ではないよ。







#pragma region 前方宣言

/// <summary>
/// ウィンドウクラス
/// </summary>
class WindowsSetup;

/// <summary>
/// DirectXクラス
/// </summary>
class DirectXSetup;

/// <summary>
/// SRV管理クラス
/// </summary>
class SrvManager;

/// <summary>
/// RTV管理クラス
/// </summary>
class RtvManager;

/// <summary>
/// ImGui管理クラス
/// </summary>
class ImGuiManager;

/// <summary>
/// パイプライン管理クラス
/// </summary>
class PipelineManager;

/// <summary>
/// 入力クラス
/// </summary>
class Input;


namespace Ellysia {
	/// <summary>
	/// オーディオクラス
	/// </summary>
	class Audio;

}

/// <summary>
/// JSON読み込み(グローバル変数)
/// </summary>
class GlobalVariables;

/// <summary>
/// レベルデータ管理クラス
/// </summary>
class LevelDataManager;


#pragma endregion




/// <summary>
/// EllysiaEngine(メインフレーム)
/// </summary>
class EllysiaEngine final{
public:
	
	/// <summary>
	/// コンストラクタ
	/// </summary>
	EllysiaEngine();
		

	/// <summary>
	/// 実行
	/// </summary>
	void Run();


	/// <summary>
	/// デストラクタ
	/// </summary>
	~EllysiaEngine() = default;


private:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// フレーム開始
	/// </summary>
	void BeginFrame();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// フレーム終わり
	/// </summary>
	void EndFrame();

	/// <summary>
	/// 解放
	/// </summary>
	void Release();

private:

	//ウィンドウクラス
	WindowsSetup* windowsSetup_ = nullptr;

	//DirectXクラス
	DirectXSetup* directXSetup_=nullptr;

	//SRV管理クラス
	SrvManager* srvManager_ = nullptr;

	//RTV管理クラス
	RtvManager* rtvManager_ = nullptr;

	//ImGui管理クラス
	ImGuiManager* imGuiManager_ = nullptr;

	//パイプライン管理クラス
	PipelineManager* pipelineManager_ = nullptr;

	//Inputクラス
	Input* input_ = nullptr;

	//オーディオクラス
	Ellysia::Audio* audio_ = nullptr;

	//JSON読み込み(グローバル変数)
	GlobalVariables* globalVariables_ = nullptr;

	//レベルデータ管理クラス
	LevelDataManager* levelDataManager_ = nullptr;
	



private:
	

	//ゲームの管理クラス
	std::unique_ptr<GameManager> gameManager_ = nullptr;

};