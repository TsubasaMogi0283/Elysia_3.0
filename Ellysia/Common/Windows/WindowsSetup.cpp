#include "WindowsSetup.h"

#include <ConvertLog.h>

#pragma comment(lib,"winmm.lib")




WindowsSetup* WindowsSetup::GetInstance() {
	static WindowsSetup instance;

	return &instance;
}



LRESULT WindowsSetup::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam){
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam)) {
		return true;
	}
	
	switch (msg) {
		//ウィンドウが破棄された
	case WM_DESTROY:
		//OSに対してアプリの終了を伝える
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, msg, wparam, lparam);

}

void WindowsSetup::OutputText(std::string& stringText){

	//wstringからstringに変換
	std::wstring newString = ConvertString::ToWString(stringText);
	
	//出力
	OutputDebugStringW(newString.c_str());

}

#pragma region Initializeに入れる関数

//ウィンドウに情報を入れる
void  WindowsSetup::RegisterWindowsClass() {
	
	//ウィンドウプロシャージャ
	windowClass_.lpfnWndProc = WindowProc;
	//ウィンドウクラス名
	windowClass_.lpszClassName = L"%s",title_;
	//インスタンドハンドル
	windowClass_.hInstance = GetModuleHandle(nullptr);
	//カーソル
	windowClass_.hCursor = LoadCursor(nullptr, IDC_ARROW);
	//ウィンドウクラス登録
	RegisterClass(&windowClass_);

	//クライアント領域サイズ
	//ウィンドウサイズを表す構造体にクライアント領域を入れる
	RECT wrc_ = { 0,0,LONG(clientWidth_) ,LONG(clientHeight_) };
	// クライアント領域を元に実際のサイズにwrcを変更
	AdjustWindowRect(&wrc_, WS_OVERLAPPEDWINDOW, false);
	// ウィンドウ生成
	hwnd_ = CreateWindow(
		windowClass_.lpszClassName,		//クラス名
		title_,					//タイトルバーの文字
		WS_OVERLAPPEDWINDOW,	//標準的なウィンドウスタイル
		CW_USEDEFAULT,			//標準X座標
		CW_USEDEFAULT,			//標準Y座標
		wrc_.right - wrc_.left, //横幅
		wrc_.bottom - wrc_.top, //縦幅
		nullptr,				//親ハンドル
		nullptr,				//メニューハンドル
		windowClass_.hInstance,			//インスタンスハンドル
		nullptr					//オプション
	);

		
	
}

void WindowsSetup::DisplayWindow() {
	//ウィンドウを表示
	ShowWindow(hwnd_, SW_SHOW);
}

#pragma endregion

//初期化
void WindowsSetup::Initialize(const wchar_t* title, int32_t clientWidth,int32_t clientHeight) {
	this->title_ = title;
	this->clientWidth_ = clientWidth;
	this->clientHeight_ = clientHeight;

	//システムタイマーの分解能を上げる
	timeBeginPeriod(1);

	//ウィンドウクラスを登録
	RegisterWindowsClass();

	//ウィンドウを表示
	DisplayWindow();
}






void WindowsSetup::WindowsMSG(MSG& msg) {
	TranslateMessage(&msg);
	DispatchMessage(&msg);
}



//ウィンドウを閉じる
void WindowsSetup::Close() {
	
	CloseWindow(hwnd_);
}


