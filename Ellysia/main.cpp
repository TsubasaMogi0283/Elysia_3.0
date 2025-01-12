#include "Ellysia.h"

//リリースチェッカー
static D3DResourceLeakChecker checker;


//main関数
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	
	//インスタンスを取得
	std::unique_ptr<Ellysia> myEngine = std::make_unique<Ellysia>();

	//実行
	myEngine->Run();


	return 0;
}

