#pragma once
#include "GameManager.h"
#include "WindowsSetup.h"
#include "DirectXSetup.h"

//Ellysia(エリシア・エレシア)
//Aile + Sheer...「エイル・エル(フランス語で自分の名前になる)」と「シア」
//sheer・・・全くの、完全な。混じりけのない、純粋な
//「Ellysia Engine」というように、後ろにEngine加えても自然だと思う

//崩〇3〇dじゃないよ


class Ellysia final{
private:
	//コンストラクタ
	Ellysia() = default;
		
	//デストラクタ
	~Ellysia();

public:
	//インスタンス
	static Ellysia* GetInstance();

	//コピーコンストラクタ禁止
	Ellysia(const Ellysia& ellysia) = delete;

	//代入演算子を無効にする
	Ellysia& operator=(const Ellysia& ellysia) = delete;


public:
	//基本の動作
	void Operate();


private:
	//初期化処理に入れる関数
	void Initialize();

 	//更新処理の中に入る関数
	void BeginFrame();

	//更新
	void Update();

	//描画
	void Draw();

	//描画処理の中に入る関数
	void EndFrame();

	//解放
	void Release();

private:
	


	GameManager* gameManager_ = nullptr;


};