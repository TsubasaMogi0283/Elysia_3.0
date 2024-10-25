#include "Ellysia.h"

//staticはdeleteの後に解放されるよ
static D3DResourceLeakChecker checker;

//main関数
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	//インスタンスを取得
	Ellysia* myEngine = Ellysia::GetInstance();

	//実行
	myEngine->Run();

	return 0;
}

