#pragma once

/**
 * @file Input.h
 * @brief 入力クラス
 * @author 茂木翼
 */

//ここでDirectXInputのバージョン設定をする
#define DIRECTINPUT_VERSION	0x0800

#include <dinput.h>
#include <Xinput.h>
#include <wrl.h>
using namespace Microsoft::WRL;


#include <WindowsSetup.h>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"xinput.lib")


/// <summary>
/// マウスの情報
/// </summary>
enum MouseInformation{
	LeftButton,
	RightButton,
	MouseWheel,
};

/// <summary>
/// 入力
/// </summary>
class Input final {
private:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Input()=default;
	
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Input()=default;

public:
	
	/// <summary>
	/// コピーコンストラクタ禁止
	/// </summary>
	/// <param name="input"></param>
	Input(const Input& input) = delete;

	/// <summary>
	/// 代入演算子を無効にする
	/// </summary>
	/// <param name="input"></param>
	/// <returns></returns>
	Input& operator=(const Input& input) = delete;

	/// <summary>
	/// シングルインスタンス
	/// </summary>
	/// <returns></returns>
	static Input* GetInstance();


public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// Stateの取得
	/// </summary>
	/// <returns></returns>
	inline XINPUT_STATE GetState()const {
		return state_;
	}



#pragma region キーボード
	/// <summary>
	/// Push状態
	/// </summary>
	/// <param name="keyNumber"></param>
	/// <returns></returns>
	bool IsPushKey(const uint8_t& keyNumber)const;

	/// <summary>
	/// Trigger状態
	/// </summary>
	/// <param name="keyNumber"></param>
	/// <returns></returns>
	bool IsTriggerKey(const uint8_t& keyNumber)const;


#pragma endregion

#pragma region マウス


	//Push状態
	bool IsPushMouse(const int32_t& keyNumber)const;

	//Trigger状態
	bool IsTriggerMouse(const int32_t& keyNumber)const;

#pragma endregion

#pragma region コントローラー
	/// <summary>
	/// コントローラーと繋がっているかどうか
	/// </summary>
	/// <returns></returns>
	bool IsConnetGamePad();

	/// <summary>
	/// Push状態
	/// </summary>
	/// <param name="button"></param>
	/// <returns></returns>
	bool IsPushButton(const int32_t& button)const;

	/// <summary>
	/// 振動の設定
	/// </summary>
	/// <param name="leftMotor"></param>
	/// <param name="rightMotor"></param>
	void SetVibration(const float& leftMotor,const float& rightMotor);
	
	/// <summary>
	/// 振動を止める
	/// </summary>
	void StopVibration();

#pragma endregion
	
	/// <summary>
	/// カーソルの表示・非表示
	/// </summary>
	/// <param name="isDisplay"></param>
	void SetIsDisplayCursor(const bool& isDisplay);

	

private:


	//DirectInputの初期化
	ComPtr<IDirectInput8> directInput_ = nullptr;

	//キーボードデバイスの生成
	ComPtr<IDirectInputDevice8> keyboard_ = nullptr;
	//マウスのデバイスを生成
	ComPtr<IDirectInputDevice8> mouseDevice_ = nullptr;
	//コントローラーのデバイスを生成
	ComPtr<IDirectInputDevice8> joyStickDevice_ = nullptr;

	//全キーの入力状態を取得する
	BYTE preKey_[256] = {};
	BYTE currentKey_[256] = {};

	//コントローラーのボタン
	//16の4乗
	static const uint32_t XINPUT_GAMEPAD_NUMBER_ = 16 * 16 * 16 * 16;
	BYTE preControllerButtons_[XUSER_MAX_COUNT][XINPUT_GAMEPAD_TRIGGER_THRESHOLD] = {};
	BYTE currentControllerButtons_[XUSER_MAX_COUNT][XINPUT_GAMEPAD_TRIGGER_THRESHOLD] = {};

	//コントローラー
	XINPUT_STATE state_ = {};


	//マウスの入力状態を取得
	DIMOUSESTATE currentMouse_ = {};
	DIMOUSESTATE preMouse_ = {};


	



};

