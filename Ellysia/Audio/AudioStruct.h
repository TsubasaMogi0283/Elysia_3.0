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
	int32_t bufferSize;
};


struct AudioInformation {

	//読み込んだオーディオの名前
	std::string fileName;

	//拡張子
	std::string extension;

	//サウンドデータ
	SoundData soundData = {};

	//サウンドボイス
	IXAudio2SourceVoice* sourceVoice;


	
	//ソースリーダー
	IMFSourceReader* sourceReader;

	//メディアデータ
	std::vector<BYTE> mediaData;

	//ハンドル
	uint32_t handle;

	//Pan用
	float left;
	float right;

};