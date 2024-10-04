#pragma once

#define XAUDIO2_HELPER_FUNCTIONS
#include <xaudio2.h>
#include "xaudio2fx.h"
#include <fstream>
#include <x3daudio.h>
#include <mmsystem.h>
#include <cassert>
#include <array>
#include <XAPOFX.h>
#pragma comment(lib, "winmm.lib")
#pragma comment(lib,"xaudio2.lib")




#include <mfapi.h>
#include <mfidl.h>


#pragma comment(lib, "Mf.lib")
#pragma comment(lib, "mfplat.lib")
#pragma comment(lib, "Mfreadwrite.lib")
#pragma comment(lib, "mfuuid.lib")

#include <wrl.h>
using Microsoft::WRL::ComPtr;


#include <complex>
#include <vector>


#include "AudioStruct.h"


class Audio {
public:
	//コンストラクタ
	Audio()=default;

	//デストラクタ
	~Audio()=default;
public:
	//インスタンスの取得
	static Audio* GetInstance();


public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

#pragma region 基本セット

	/// <summary>
	/// 読み込み
	/// </summary>
	/// <param name="fileName"></param>
	/// <returns></returns>
	static uint32_t LoadWave(const char* fileName);

	/// <summary>
	/// エフェクト版の読み込み
	/// </summary>
	/// <param name="fileName"></param>
	/// <param name="effectType"></param>
	/// <returns></returns>
	static uint32_t LoadWave(const char* fileName, uint32_t effectType);

	/// <summary>
	/// MP3読み込み
	/// </summary>
	/// <param name="fileName"></param>
	/// <returns></returns>
	static uint32_t LoadMP3(const WCHAR* fileName);

	/// <summary>
	/// 再生
	/// </summary>
	/// <param name="audioHandle"></param>
	/// <param name="isLoop"></param>
	void PlayWave(uint32_t audioHandle, bool isLoop);
	
	/// <summary>
	/// 再生(ループ回数指定)
	/// </summary>
	/// <param name="audioHandle"></param>
	/// <param name="loopCount"></param>
	void PlayWave(uint32_t audioHandle, int32_t loopCount);

	// <summary>
	// MP3再生
	// </summary>
	// <param name="audioHandle"></param>
	// <param name="isLoop"></param>
	void PlayMP3(uint32_t audioHandle,bool isLoop);

	// <summary>
	// MP3再生(ループ回数設定版)
	// </summary>
	// <param name="audioHandle"></param>
	// <param name="loopCount"></param>
	void PlayMP3(uint32_t audioHandle, uint32_t loopCount);


	/// <summary>
	/// 一時停止
	/// </summary>
	/// <param name="ハンドル名"></param>
	void PauseWave(uint32_t audioHandle);

	/// <summary>
	/// 再開
	/// </summary>
	/// <param name="ハンドル名"></param>
	void ResumeWave(uint32_t audioHandle);

	/// <summary>
	/// 停止
	/// </summary>
	/// <param name="ハンドル名"></param>
	void StopWave(uint32_t audioHandle);


#pragma endregion


#pragma region ループ


	/// <summary>
	/// ループから抜ける
	/// </summary>
	/// <param name="ハンドル名"></param>
	void ExitLoop(uint32_t audioHandle);


	/// <summary>
	/// 再生(後半ループ)
	/// </summary>
	/// <param name="ハンドル名"></param>
	/// <param name="始まる位置(秒)"></param>
	void AfterLoopPlayWave(uint32_t audioHandle, float second);

	/// <summary>
	/// 再生(前半ループ)
	/// </summary>
	/// <param name="ハンドル名"></param>
	/// <param name="ループ間の長さ(秒)"></param>
	void BeforeLoopPlayWave(uint32_t audioHandle, float lengthSecond);


	/// <summary>
	/// 部分ループ
	/// </summary>
	/// <param name="ハンドル名"></param>
	/// <param name="開始位置(秒)"></param>
	/// <param name="ループの長さ(秒)"></param>
	void PartlyLoopPlayWave(uint32_t audioHandle, float start, float lengthSecond);


#pragma endregion

	/// <summary>
	/// 音量調節
	/// </summary>
	/// <param name="ハンドル名"></param>
	/// <param name="音量"></param>
	void ChangeVolume(uint32_t audioHandle, float volume);

#pragma region ピッチ系

	/// <summary>
	/// 音の高さの変更(滑らか版)
	/// </summary>
	/// <param name="ハンドル名"></param>
	/// <param name="値"></param>
	void ChangeFrequency(uint32_t audioHandle, float ratio);


	/// <summary>
	/// ピッチの変更
	/// シンセのように出来るよ
	/// </summary>
	/// <param name="ハンドル名"></param>
	/// <param name="値"></param>
	void ChangePitch(uint32_t audioHandle, int32_t scale);

	void StretchAndPitch(uint32_t audioHandle, float timeRatio, float pitchRatio);

#pragma endregion

	/// <summary>
	/// Pan振り
	/// </summary>
	/// <param name="ハンドル名"></param>
	/// <param name="Panの値"></param>
	void SetPan(uint32_t audioHandle, float_t pan);

	
#pragma region フィルター

	//単極は必要無いかな

	/// <summary>
	/// ローパスフィルター
	/// </summary>
	/// <param name="ハンドル名"></param>
	/// <param name="CutOffの値"></param>
	void SetLowPassFilter(uint32_t audioHandle, float cutOff);

	/// <summary>
	/// ローパスフィルター(Qあり)
	/// </summary>
	/// <param name="ハンドル名"></param>
	/// <param name="CutOffの値"></param>
	/// <param name="oneOverQ"></param>
	void SetLowPassFilter(uint32_t audioHandle, float cutOff,float oneOverQ);



	/// <summary>
	/// ハイパスフィルター
	/// </summary>
	/// <param name="ハンドル名"></param>
	/// <param name="CutOffの値"></param>
	void SetHighPassFilter(uint32_t audioHandle, float cutOff);

	/// <summary>
	/// ハイパスフィルター(Qあり)
	/// </summary>
	/// <param name="ハンドル名"></param>
	/// <param name="CutOffの値"></param>
	/// <param name="oneOverQ"></param>
	void SetHighPassFilter(uint32_t audioHandle, float cutOff, float oneOverQ);

	/// <summary>
	/// バンドパス
	/// </summary>
	/// <param name="ハンドル名"></param>
	/// <param name="CutOffの値"></param>
	void SetBandPassFilter(uint32_t audioHandle, float cutOff);

	/// <summary>
	/// バンドパス(Qあり)
	/// </summary>
	/// <param name="ハンドル名"></param>
	/// <param name="CutOffの値"></param>
	void SetBandPassFilter(uint32_t audioHandle, float cutOff, float oneOverQ);


	/// <summary>
	/// ノッチフィルター
	/// </summary>
	/// <param name="ハンドル名"></param>
	/// <param name="CutOffの値"></param>
	void SetNotchFilter(uint32_t audioHandle, float cutOff);

	/// <summary>
	/// ノッチフィルター
	/// </summary>
	/// <param name="ハンドル名"></param>
	/// <param name="CutOffの値"></param>
	void SetNotchFilter(uint32_t audioHandle, float cutOff, float oneOverQ);

#pragma endregion



	void SendChannels(uint32_t audioHandle,uint32_t channelNumber);

	//リバーブ
	void CreateReverb(uint32_t audioHandle, uint32_t channel);

	//エフェクトの効果を無効にする
	void OffEffect(uint32_t audioHandle);

	//エフェクトの効果を有効にする
	void OnEffect(uint32_t audioHandle);



	


};