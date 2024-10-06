#pragma once
#include <cstdint>
#include <xaudio2.h>
#include <string>
#include <mfreadwrite.h>

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
	std::string fileName = {};

	//サウンドデータ
	SoundData soundData = {};

	//サウンドボイス
	IXAudio2SourceVoice* sourceVoice = nullptr;


	
	//ソースリーダー
	IMFSourceReader* sourceReader = nullptr;

	std::vector<BYTE> mediaData;

	//ハンドル
	uint32_t handle = 0;

	//Pan用
	float outputMatrix[8] = {};
	float left = 0.0f;
	float right = 0.0f;

};