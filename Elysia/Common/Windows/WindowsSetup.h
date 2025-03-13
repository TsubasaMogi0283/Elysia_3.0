#pragma once

/**
 * @file WindowsSetup.h
 * @brief Windowsクラス
 * @author 茂木翼
 */

#include <Windows.h>
#include <WinUser.h>
#include <cstdint>
#include <string>

#include <imgui.h>
#include <imgui_impl_dx12.h>
#include <imgui_impl_win32.h>

//外部の,、つまり自分が作ったものではないファイルなどは
//<>でインクルードさせた方が良い
//その他自分で作ったものは""でインクルードさせてね

//extern...グローバル変数を共有する

/// <summary>
///	ウィンドウプロシージャで使うもの
/// </summary>
/// <param name="hwnd"></param>
/// <param name="msg"></param>
/// <param name="wParam"></param>
/// <param name="lParam"></param>
/// <returns></returns>
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam);

/// <summary>
/// Elysia
/// </summary>
namespace Elysia {

	/// <summary>
	/// Windowsの設定(シングルトン)
	/// </summary>
	class WindowsSetup final {
	private:
		//インスタンスを作れないようにする
		//コンストラクタをprivateに

		/// <summary>
		/// コンストラクタ
		/// </summary>
		WindowsSetup() = default;

		/// <summary>
		/// デストラクタ
		/// </summary>
		~WindowsSetup() = default;

	public:

#pragma region 禁止事項

		/// <summary>
		/// コピーコンストラクタ禁止
		/// </summary>
		/// <param name="winApp"></param>
		WindowsSetup(const WindowsSetup& winApp) = delete;

		/// <summary>
		/// 代入演算子を無効にする
		/// </summary>
		/// <param name="winApp"></param>
		/// <returns></returns>
		WindowsSetup& operator=(const WindowsSetup& winApp) = delete;

#pragma endregion

		/// <summary>
		/// インスタンスの取得
		/// </summary>
		/// <returns></returns>
		static WindowsSetup* GetInstance();


		/// <summary>
		/// ウィンドウプロシージャ
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
		/// <param name="title">タイトル名</param>
		/// <param name="clientWidth">横サイズ</param>
		/// <param name="clientHeight">縦サイズ</param>
		void Initialize(const wchar_t* title, int32_t clientWidth, int32_t clientHeight);

		/// <summary>
		/// メッセージ
		/// </summary>
		/// <param name="msg">メッセージ</param>
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
		uint32_t GetClientWidth() const {
			return clientWidth_;
		}

		/// <summary>
		/// クライアントの縦幅
		/// </summary>
		/// <returns></returns>
		uint32_t GetClientHeight() const {
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
		HINSTANCE GetHInstance() const {
			return windowClass_.hInstance;
		}



	private:

		//タイトルバー
		const wchar_t* title_ = L"Ellysia";

		//クライアントのサイズ
		uint32_t clientWidth_ = 0;
		uint32_t clientHeight_ = 0;

		//ウィンドウハンドル
		HWND hwnd_ = 0;

		//ウィンドウクラス
		WNDCLASS windowClass_{};



	};


};
