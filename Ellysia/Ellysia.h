#pragma once
#include "GameManager.h"
#include "WindowsSetup.h"
#include "DirectXSetup.h"

//Ellysia(エリシア・エレシア)
//Aile + Sheer...「エイル・エル(フランス語で自分の名前になる)」と「シア」
//sheer・・・全くの、完全な。混じりけのない、純粋な
//「Ellysia Engine」というように、後ろにEngine加えても自然だと思う

//崩〇3〇dじゃないよ

/// <summary>
/// エンジン
/// mainにベタ書きではなくこっちに書いてあるよ
/// </summary>
class Ellysia final{
private:
	
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Ellysia() = default;
		
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Ellysia()=default;

public:
	/// <summary>
	/// インスタンスの取得
	/// </summary>
	/// <returns></returns>
	static Ellysia* GetInstance();

	//コピーコンストラクタ禁止
	Ellysia(const Ellysia& ellysia) = delete;

	//代入演算子を無効にする
	Ellysia& operator=(const Ellysia& ellysia) = delete;


public:
	/// <summary>
	/// 実行
	/// </summary>
	void Run();


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
	
	//ゲームの管理クラス
	std::unique_ptr<GameManager> gameManager_ = nullptr;

};