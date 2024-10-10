#pragma once

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


//0,1,2,3とか分かりにくいのでenumで分かりやすくしたい
enum MouseInformation{
	LeftButton,
	RightButton,
	MouseWheel,
};

class Input {
private:

	//コンストラクタ
	Input()=default;
	
	//デストラクタ
	~Input()=default;

public:
	
	//コピーコンストラクタ禁止
	Input(const Input& input) = delete;

	//代入演算子を無効にする
	Input& operator=(const Input& input) = delete;

	//シングルインスタンス
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


	inline XINPUT_STATE GetState()const {
		return state_;
	}



#pragma region キーボード
	//Push状態
	bool IsPushKey(uint8_t keyNumber);

	//Trigger状態
	bool IsTriggerKey(uint8_t keyNumber);

	//Releaseいるかな・・・
	//必要になったら追加する

#pragma endregion

#pragma region マウス


	//Push状態
	bool IsPushMouse(int32_t keyNumber);

	//Trigger状態
	bool IsTriggerMouse(int32_t keyNumber);

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
	bool IsPushButton(int32_t button);

	void SetVibration(float leftMotor,float rightMotor);
	void StopVibration();

#pragma endregion
	
	/// <summary>
	/// カーソルの表示・非表示
	/// </summary>
	/// <param name="isDisplay"></param>
	void SetIsDisplayCursor(bool isDisplay);

	

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



	//マウスの入力状態を取得
	DIMOUSESTATE currentMouse_ = {};
	DIMOUSESTATE preMouse_ = {};


	//コントローラー
	XINPUT_STATE state_{};




};

