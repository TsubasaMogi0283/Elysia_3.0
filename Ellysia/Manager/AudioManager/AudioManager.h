#pragma once
#include <cstdint>
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

//LoadAudioの時に使う
enum AudioEffectType {
	Filter,
	Reverb,
};

class AudioManager final{
private:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	AudioManager()=default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~AudioManager()=default;
public:
	/// <summary>
	/// インスタンスの取得
	/// </summary>
	/// <returns></returns>
	static AudioManager* GetInstance();

	/// <summary>
	/// コピーコンストラクタ禁止
	/// </summary>
	/// <param name="obj"></param>
	AudioManager(const AudioManager& obj) = delete;

	/// <summary>
	/// 代入演算子を無効にする
	/// </summary>
	/// <param name="obj"></param>
	/// <returns></returns>
	AudioManager& operator=(const AudioManager& obj) = delete;


private:
	static void CreateSubmixVoice(uint32_t channel);

	//音声データの開放
	void SoundUnload(uint32_t soundDataHandle);

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	//解放
	void Release();

public:
	//読み込み
	static uint32_t LoadWave(const char* fileName);

	//エフェクト版の読み込み
	static uint32_t LoadWave(const char* fileName, uint32_t effectType);

	//MP3の読み込み
	static uint32_t LoadMP3(const WCHAR* fileName);


public:

	inline IXAudio2MasteringVoice* GetMasterVoice() {
		return masterVoice_;
	}


	/// <summary>
	/// audioデータを取得
	/// </summary>
	/// <param name="handle"></param>
	/// <returns></returns>
	inline AudioInformation GetAudioInformation(uint32_t handle) {
		return audioInformation_[handle];
	}

	/// <summary>
	/// サブミックスボイスを取得
	/// </summary>
	/// <param name="channel"></param>
	/// <returns></returns>
	inline IXAudio2SubmixVoice* GetSubmixVoice(uint32_t channel) {
		return submixVoice_[channel];
	}



	inline DWORD GetDwChannelMask() {
		return dwChannelMask_;
	}



	


private:
	//IXAudio2はCOMオブジェクトなのでComPtr管理
	ComPtr<IXAudio2> xAudio2_ = nullptr;
	//マスターボイス
	//最終的にここでまとめるよ(スピーカーみたいな感じだね)
	IXAudio2MasteringVoice* masterVoice_ = nullptr;

	DWORD dwChannelMask_ = {};
	
	//Reverb
	IUnknown* pXAPO_ = nullptr;


	static uint32_t audioIndex;


	//構造体版
	//Texturemanagerとだいたい同じ感じにした
	//音声データの最大数
	static const int SOUND_DATE_MAX_ = 256;
	std::array<AudioInformation, SOUND_DATE_MAX_> audioInformation_{};

	//サブミックス(バス)
	static const int SUBMIXVOICE_AMOUNT_ = 64;
	std::array<IXAudio2SubmixVoice*, SUBMIXVOICE_AMOUNT_> submixVoice_{};


};

