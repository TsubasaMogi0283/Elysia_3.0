#pragma once
#include <cstdint>
#include <xaudio2.h>
#include <fstream>
#include <cassert>

#include <x3daudio.h>
#include <mmsystem.h>
#include <cassert>
#include <array>

#pragma comment(lib, "winmm.lib")
#pragma comment(lib,"xaudio2.lib")

#include <wrl.h>
using Microsoft::WRL::ComPtr;

class AudioManager{
private:
	//チャンク...データの塊みたいなもの
	//チャンクヘッダ
	struct ChunkHeader {
		//チャンク毎のID
		char id[4];
		//チャンクサイズ
		int32_t size;
	};

	//FMTチャンク
	struct FormatChunk {
		//fmt
		ChunkHeader chunk;
		//波形フォーマット
		WAVEFORMATEX fmt;

	};

	struct RiffHeader {
		//RIFF
		ChunkHeader chunk;
		//WAVE
		char type[4];
	};

	//音声データ
	struct SoundData {
		//波形フォーマット
		WAVEFORMATEX wfex;

		//バッファの先頭アドレス
		BYTE* pBuffer;

		//バッファのサイズ
		unsigned int bufferSize;
	};

	struct AudioInformation {

		//読み込んだテクスチャの名前
		std::string name_ = {};

		//サウンドデータ
		SoundData soundData_ = {};

		//波形フォーマットを基にSourceVoiceの生成
		IXAudio2SourceVoice* pSourceVoice_ = nullptr;

		//ハンドル
		uint32_t audioHandle_ = 0;

		//テクスチャハンドル
		uint32_t handle_ = 0;
	};


	

private:
	//コンストラクタ
	AudioManager();

	//デストラクタ
	~AudioManager();
public:
	//インスタンスの取得
	static AudioManager* GetInstance();

	//コピーコンストラクタ禁止
	AudioManager(const AudioManager& audio) = delete;

	//代入演算子を無効にする
	AudioManager& operator=(const AudioManager& audio) = delete;


public:

	//オーディオ情報のゲッター
	//これをAudioクラスに持っていく
	const AudioInformation& GetAudioInformation(int index) {
		if (index >= 0 && index < SOUND_DATE_MAX_) {
			return audioInformation_[index];
		}
		else {
			//停止
			assert(index >= 0 && index < SOUND_DATE_MAX_);
		}

	}

	//読み込み
	//全体で共有したいからstaticにしているよ
	static uint32_t LoadWave(const char* fileName);



private:
	////読み込んだテクスチャの名前
	//std::string name_ = {};

	////サウンドデータ
	//SoundData soundData_ = {};

	////波形フォーマットを基にSourceVoiceの生成
	//IXAudio2SourceVoice* pSourceVoice_ = nullptr;

	////ハンドル
	//uint32_t audioHandle_ = 0;

	////テクスチャハンドル
	//uint32_t handle_ = 0;

	//音声データの最大数
	static const int SOUND_DATE_MAX_ = 256;

	const AudioInformation audioInformation_[SOUND_DATE_MAX_]= {};
};

