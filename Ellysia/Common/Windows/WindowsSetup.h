#pragma once

#include <Windows.h>
#include <WinUser.h>
#include <cstdint>
#include <string>

#include <imgui.h>
#include <imgui_impl_dx12.h>
#include <imgui_impl_win32.h>

//外部のつまり自分が作ったものではないファイルなどは
//<>でインクルードさせた方が良いよ

//その他自分で作ったものは""でインクルードさせてね

//extern...グローバル変数を共有する

/// <summary>
/// ImGuiで使うもの
/// </summary>
/// <param name="hwnd"></param>
/// <param name="msg"></param>
/// <param name="wParam"></param>
/// <param name="lParam"></param>
/// <returns></returns>
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam);

/// <summary>
/// Windowsの設定(シングルトン)
/// </summary>
class WindowsSetup final{
private:
	//インスタンスを作れないようにする
	//コンストラクタをprivateに
	
	/// <summary>
	/// コンストラクタ
	/// </summary>
	WindowsSetup()=default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~WindowsSetup() = default;

public:

	#pragma region 禁止事項

	//コピーコンストラクタ禁止
	WindowsSetup(const WindowsSetup& winApp) = delete;

	//代入演算子を無効にする
	WindowsSetup& operator=(const WindowsSetup& winApp) = delete;

	#pragma endregion

	//インスタンスにアクセス可能な関数を追加
	//静的メンバ関数にする。クラス名を指定すればアクセスできる
	static WindowsSetup* GetInstance();


	/// <summary>
	/// ウィンドウプロシージャ(コールバック関数だよ)
	/// </summary>
	/// <param name="hwnd"></param>
	/// <param name="msg"></param>
	/// <param name="wparam"></param>
	/// <param name="lparam"></param>
	/// <returns></returns>
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	/// <summary>
	/// 出力の所に文字を出す
	/// </summary>
	/// <param name="stringText"></param>
	static void OutputText(std::string& stringText);

private:

	/// <summary>
	/// //ウィンドウクラスを登録
	/// </summary>
	void RegisterWindowsClass();

	/// <summary>
	/// Windowを表示
	/// </summary>
	void DisplayWindow();


public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="title"></param>
	/// <param name="clientWidth"></param>
	/// <param name="clientHeight"></param>
	void Initialize(const wchar_t* title, int32_t clientWidth,int32_t clientHeight);

	/// <summary>
	/// メッセージ
	/// </summary>
	/// <param name="msg"></param>
	void WindowsMSG(MSG& msg);

	/// <summary>
	/// 閉じる
	/// </summary>
	void Close();

public:

	/// <summary>
	/// クライアントの横幅
	/// </summary>
	/// <returns></returns>
	uint32_t GetClientWidth() const{
		return clientWidth_;
	}

	/// <summary>
	/// クライアントの縦幅
	/// </summary>
	/// <returns></returns>
	uint32_t GetClientHeight() const{
		return clientHeight_;
	}

	/// <summary>
	/// Hwndの取得
	/// </summary>
	/// <returns></returns>
	HWND GetHwnd()const {
		return hwnd_;
	}

	/// <summary>
	/// hInstanceの取得
	/// </summary>
	/// <returns></returns>
	HINSTANCE GetHInstance() const{
		return windowClass_.hInstance;
	}



private:

	//タイトルバー
	const wchar_t* title_=L"Ellysia";
	
	//クライアントのサイズ
	uint32_t clientWidth_=0;
	uint32_t clientHeight_=0;
	
	//ウィンドウハンドル
	HWND hwnd_=0;

	//ウィンドウクラス
	WNDCLASS windowClass_{};

	

};

