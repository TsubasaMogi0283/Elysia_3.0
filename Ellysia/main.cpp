#include "Ellysia.h"

//リリースチェッカー
static D3DResourceLeakChecker checker;

//main関数
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	//フレームワークの生成
	std::unique_ptr<Ellysia::Framework> ellysia = std::make_unique<Ellysia::Framework>();

	//実行
	ellysia->Run();

	//終了
	return 0;
}

