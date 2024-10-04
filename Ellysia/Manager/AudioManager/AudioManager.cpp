#include "AudioManager.h"


uint32_t AudioManager::audioIndex = 0;


AudioManager* AudioManager::GetInstance(){
    static AudioManager instance;
    return &instance;
}


void AudioManager::CreateSubmixVoice(uint32_t channel) {
	uint32_t channels = channel;
	uint32_t sampleRate = 44100;



	HRESULT hr = {};
	hr = AudioManager::GetInstance()->xAudio2_->CreateSubmixVoice(&AudioManager::GetInstance()->submixVoice_[channel], channels, sampleRate);
	assert(SUCCEEDED(hr));

}

//初期化
//これはDirecX初期化の後に入れてね
void AudioManager::Initialize() {

	//MediaFundationの初期化
	MFStartup(MF_VERSION, MFSTARTUP_NOSOCKET);

	//XAudioのエンジンのインスタンスを生成
	HRESULT hr;
	hr = XAudio2Create(&xAudio2_, 0, XAUDIO2_DEFAULT_PROCESSOR);
	assert(SUCCEEDED(hr));

	//マスターボイスを生成
	hr = xAudio2_->CreateMasteringVoice(&masterVoice_);
	assert(SUCCEEDED(hr));

	//スピーカ構成を取得
	masterVoice_->GetChannelMask(&dwChannelMask_);





	//サブミックスボイス(DTMでのバス)をここで作る
	//FLStudioと同じように128あれば良いなと思ったが
	//64くらいあれば十分でしょう。多すぎてもメモリの無駄になってしまうし
	for (int i = 1; i < SUBMIXVOICE_AMOUNT_; ++i) {

		CreateSubmixVoice(i);
	}




}

#pragma region 実際に使う関数

#pragma region 基本セット
//読み込み
uint32_t AudioManager::LoadWave(const char* fileName) {

	//16,24,32bitは読み込み出来た
	//64bitも読み込み出来るようにしたいと思ったが一般的に使われないらしい
	//だから32が最大で良いかも。
	//64bitを書き出せるCakewslkすご


	//一度読み込んだものは２度読み込まず返すだけ
	for (int i = 0; i < SOUND_DATE_MAX_; i++) {
		if (AudioManager::GetInstance()->audioInformation_[i].name_ == fileName) {
			return AudioManager::GetInstance()->audioInformation_[i].audioHandle_;
		}
	}
	//indexを足していく
	int index = audioIndex;
	audioIndex++;
	

	//記録
	AudioManager::GetInstance()->audioInformation_[index].name_ = fileName;
	AudioManager::GetInstance()->audioInformation_[index].audioHandle_ = index;


#pragma region １,ファイルオープン
	//ファイル入力ストリームのインスタンス
	std::ifstream file;
	//.wavファイルをバイナリモードで開く
	file.open(fileName, std::ios_base::binary);
	//ファイルオープン失敗を検出する
	assert(file.is_open());

#pragma endregion

#pragma region ２,wavデータ読み込み

	//RIFFヘッダーの読み込み
	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));
	//ファイルがRIFFかチェック
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0) {
		assert(0);
	}
	//タイプがWAVEかチェック
	if (strncmp(riff.type, "WAVE", 4) != 0) {
		assert(0);
	}

	//Formatチャンクの読み込み
	FormatChunk format = {};
	//チャンクヘッダーの確認
	file.read((char*)&format, sizeof(ChunkHeader));
	//何かここ空白入れないとダメらしい
	//後ろが4だからかな・・
	if (strncmp(format.chunk.id, "fmt ", 4) != 0) {
		assert(0);
	}

	//チャンク本体の読み込み
	assert(format.chunk.size <= sizeof(format.fmt));
	file.read((char*)&format.fmt, format.chunk.size);


	//Dataチャンクの読み込み
	ChunkHeader data;
	file.read((char*)&data, sizeof(data));
	//JUNKチャンクを検出した場合
	if (strncmp(data.id, "JUNK", 4) == 0) {
		//読み込み位置をJUNKチャンクの終わりまで進める
		file.seekg(data.size, std::ios_base::cur);
		//再読み込み
		file.read((char*)&data, sizeof(data));

	}

	//メインのデータチャンク
	if (strncmp(data.id, "data", 4) != 0) {
		assert(0);
	}

	//Dataチャンクのデータ部(波形データ)の読み込み
	char* pBuffer = new char[data.size];
	file.read(pBuffer, data.size);
#pragma endregion

#pragma region ３,Waveファイルを閉じる
	file.close();


#pragma endregion




#pragma region 読み込んだ音声データを返す

	AudioManager::GetInstance()->audioInformation_[index].soundData_.wfex = format.fmt;
	AudioManager::GetInstance()->audioInformation_[index].soundData_.pBuffer = reinterpret_cast<BYTE*>(pBuffer);
	AudioManager::GetInstance()->audioInformation_[index].soundData_.bufferSize = data.size;


	//波形フォーマットを基にSourceVoiceの生成
	HRESULT hr{};
	hr = AudioManager::GetInstance()->xAudio2_->CreateSourceVoice(
		&AudioManager::GetInstance()->audioInformation_[index].pSourceVoice_,
		&AudioManager::GetInstance()->audioInformation_[index].soundData_.wfex);
	assert(SUCCEEDED(hr));


	//フィルターを使う場合
	//後でFilterを使う場合のロードを作りたい
	hr = AudioManager::GetInstance()->xAudio2_->CreateSourceVoice(
		&AudioManager::GetInstance()->audioInformation_[index].pSourceVoice_,
		&AudioManager::GetInstance()->audioInformation_[index].soundData_.wfex, XAUDIO2_VOICE_USEFILTER, 16.0f);

	assert(SUCCEEDED(hr));

	return index;

#pragma endregion



}

uint32_t AudioManager::LoadWave(const char* fileName, uint32_t effectType) {
	effectType;
	//16,24,32bitは読み込み出来た
	//64bitも読み込み出来るようにしたいと思ったが一般的に使われないらしい
	//だから32が最大で良いかも。
	//64bitを書き出せるCakewslkすご


	//一度読み込んだものは２度読み込まず返すだけ
	for (int i = 0; i < SOUND_DATE_MAX_; i++) {
		if (AudioManager::GetInstance()->audioInformation_[i].name_ == fileName) {
			return AudioManager::GetInstance()->audioInformation_[i].audioHandle_;
		}
	}
	//audioHandle_++;
	audioIndex++;

	//記録
	AudioManager::GetInstance()->audioInformation_[audioIndex].name_ = fileName;
	AudioManager::GetInstance()->audioInformation_[audioIndex].audioHandle_ = audioIndex;


#pragma region １,ファイルオープン
	//ファイル入力ストリームのインスタンス
	std::ifstream file;
	//.wavファイルをバイナリモードで開く
	file.open(fileName, std::ios_base::binary);
	//ファイルオープン失敗を検出する
	assert(file.is_open());

#pragma endregion

#pragma region ２,wavデータ読み込み

	//RIFFヘッダーの読み込み
	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));
	//ファイルがRIFFかチェック
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0) {
		assert(0);
	}
	//タイプがWAVEかチェック
	if (strncmp(riff.type, "WAVE", 4) != 0) {
		assert(0);
	}

	//Formatチャンクの読み込み
	FormatChunk format = {};
	//チャンクヘッダーの確認
	file.read((char*)&format, sizeof(ChunkHeader));
	//何かここ空白入れないとダメらしい
	//後ろが4だからかな・・
	if (strncmp(format.chunk.id, "fmt ", 4) != 0) {
		assert(0);
	}

	//チャンク本体の読み込み
	assert(format.chunk.size <= sizeof(format.fmt));
	file.read((char*)&format.fmt, format.chunk.size);


	//Dataチャンクの読み込み
	ChunkHeader data;
	file.read((char*)&data, sizeof(data));
	//JUNKチャンクを検出した場合
	if (strncmp(data.id, "JUNK", 4) == 0) {
		//読み込み位置をJUNKチャンクの終わりまで進める
		file.seekg(data.size, std::ios_base::cur);
		//再読み込み
		file.read((char*)&data, sizeof(data));

	}

	//メインのデータチャンク
	if (strncmp(data.id, "data", 4) != 0) {
		assert(0);
	}

	//Dataチャンクのデータ部(波形データ)の読み込み
	char* pBuffer = new char[data.size];
	file.read(pBuffer, data.size);
#pragma endregion

#pragma region ３,Waveファイルを閉じる
	file.close();


#pragma endregion




#pragma region 読み込んだ音声データを返す

	AudioManager::GetInstance()->audioInformation_[audioIndex].soundData_.wfex = format.fmt;
	AudioManager::GetInstance()->audioInformation_[audioIndex].soundData_.pBuffer = reinterpret_cast<BYTE*>(pBuffer);
	AudioManager::GetInstance()->audioInformation_[audioIndex].soundData_.bufferSize = data.size;


	//波形フォーマットを基にSourceVoiceの生成
	HRESULT hr{};

	hr = AudioManager::GetInstance()->xAudio2_->CreateSourceVoice(
		&AudioManager::GetInstance()->audioInformation_[audioIndex].pSourceVoice_,
		&AudioManager::GetInstance()->audioInformation_[audioIndex].soundData_.wfex, XAUDIO2_VOICE_USEFILTER, 16.0f);

	assert(SUCCEEDED(hr));

	return audioIndex;

#pragma endregion


}

uint32_t AudioManager::LoadMP3(const WCHAR* fileName) {
	//効果音系にはMP3は良いけど
	//ループにはおすすめできないらしい。末端に無音が入るため。





	//一度読み込んだものは２度読み込まず返すだけ
	for (int i = 0; i < SOUND_DATE_MAX_; i++) {
		if (AudioManager::GetInstance()->audioInformation_[i].mp3FileName_ == fileName) {
			return AudioManager::GetInstance()->audioInformation_[i].audioHandle_;
		}
	}

	//一度読み込んだものは２度読み込まず返すだけ
//	for (int i = 0; i < SOUND_DATE_MAX_; i++) {
//		if (Audio::GetInstance()->audioInformation_[i].mp3FileName_ == fileName) {
//			return Audio::GetInstance()->audioInformation_[i].audioHandle_;
//		}
//	}


	int index = audioIndex;
	audioIndex++;

	//記録
	AudioManager::GetInstance()->audioInformation_[index].mp3FileName_ = fileName;
	AudioManager::GetInstance()->audioInformation_[index].audioHandle_ = index;

	HRESULT hr = {};

	//ソースリーダーの作成
	hr = MFCreateSourceReaderFromURL(fileName, nullptr, &AudioManager::GetInstance()->audioInformation_[index].pSourceReader_);
	assert(SUCCEEDED(hr));
	//audioIndex++;
	//
	//	//記録
	//	Audio::GetInstance()->audioInformation_[audioIndex].mp3FileName_ = fileName;
	//	Audio::GetInstance()->audioInformation_[audioIndex].audioHandle_ = audioIndex;
	//
	//	HRESULT hr = {};
	//
	//	//ソースリーダーの作成
	//	hr = MFCreateSourceReaderFromURL(fileName, nullptr, &Audio::GetInstance()->audioInformation_[audioIndex].pSourceReader_);
	//	assert(SUCCEEDED(hr));
	//


	//メディアタイプの取得
	IMFMediaType* pMFMediaType{ nullptr };
	MFCreateMediaType(&pMFMediaType);
	pMFMediaType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio);
	pMFMediaType->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM);
	AudioManager::GetInstance()->audioInformation_[index].pSourceReader_->SetCurrentMediaType(DWORD(MF_SOURCE_READER_FIRST_AUDIO_STREAM), nullptr, pMFMediaType);

	pMFMediaType->Release();
	pMFMediaType = nullptr;
	AudioManager::GetInstance()->audioInformation_[index].pSourceReader_->GetCurrentMediaType(DWORD(MF_SOURCE_READER_FIRST_AUDIO_STREAM), &pMFMediaType);

	//オーディオデータ形式の作成
	WAVEFORMATEX* waveFormat{};
	MFCreateWaveFormatExFromMFMediaType(pMFMediaType, &waveFormat, nullptr);



	//
	//
	//	//メディアタイプの取得
	//	IMFMediaType* pMFMediaType{ nullptr };
	//	MFCreateMediaType(&pMFMediaType);
	//	pMFMediaType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio);
	//	pMFMediaType->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM);
	//	Audio::GetInstance()->audioInformation_[audioIndex].pSourceReader_->SetCurrentMediaType(DWORD(MF_SOURCE_READER_FIRST_AUDIO_STREAM), nullptr, pMFMediaType);
	//
	//	pMFMediaType->Release();
	//	pMFMediaType = nullptr;
	//	Audio::GetInstance()->audioInformation_[audioIndex].pSourceReader_->GetCurrentMediaType(DWORD(MF_SOURCE_READER_FIRST_AUDIO_STREAM), &pMFMediaType);
	//
	//	//オーディオデータ形式の作成
	//	WAVEFORMATEX* waveFormat{};
	//	MFCreateWaveFormatExFromMFMediaType(pMFMediaType, &waveFormat, nullptr);
	//
	//
	//

	while (true)
	{
		IMFSample* pMFSample{ nullptr };
		DWORD dwStreamFlags{ 0 };
		AudioManager::GetInstance()->audioInformation_[index].pSourceReader_->ReadSample(DWORD(MF_SOURCE_READER_FIRST_AUDIO_STREAM), 0, nullptr, &dwStreamFlags, nullptr, &pMFSample);

		if (dwStreamFlags & MF_SOURCE_READERF_ENDOFSTREAM)
		{
			break;
		}

		IMFMediaBuffer* pMFMediaBuffer{ nullptr };
		pMFSample->ConvertToContiguousBuffer(&pMFMediaBuffer);

		BYTE* pBuffer{ nullptr };
		DWORD cbCurrentLength{ 0 };
		pMFMediaBuffer->Lock(&pBuffer, nullptr, &cbCurrentLength);

		AudioManager::GetInstance()->audioInformation_[index].mediaData.resize(AudioManager::GetInstance()->audioInformation_[index].mediaData.size() + cbCurrentLength);
		memcpy(AudioManager::GetInstance()->audioInformation_[index].mediaData.data() + AudioManager::GetInstance()->audioInformation_[index].mediaData.size() - cbCurrentLength, pBuffer, cbCurrentLength);

		pMFMediaBuffer->Unlock();

		pMFMediaBuffer->Release();
		pMFSample->Release();
	}


	//	while (true)
	//	{
	//		IMFSample* pMFSample{ nullptr };
	//		DWORD dwStreamFlags{ 0 };
	//		Audio::GetInstance()->audioInformation_[audioIndex].pSourceReader_->ReadSample(DWORD(MF_SOURCE_READER_FIRST_AUDIO_STREAM), 0, nullptr, &dwStreamFlags, nullptr, &pMFSample);
	//
	//		if (dwStreamFlags & MF_SOURCE_READERF_ENDOFSTREAM)
	//		{
	//			break;
	//		}
	//
	//		IMFMediaBuffer* pMFMediaBuffer{ nullptr };
	//		pMFSample->ConvertToContiguousBuffer(&pMFMediaBuffer);
	//
	//		BYTE* pBuffer{ nullptr };
	//		DWORD cbCurrentLength{ 0 };
	//		pMFMediaBuffer->Lock(&pBuffer, nullptr, &cbCurrentLength);
	//
	//		Audio::GetInstance()->audioInformation_[audioIndex].mediaData.resize(Audio::GetInstance()->audioInformation_[audioIndex].mediaData.size() + cbCurrentLength);
	//		memcpy(Audio::GetInstance()->audioInformation_[audioIndex].mediaData.data() + Audio::GetInstance()->audioInformation_[audioIndex].mediaData.size() - cbCurrentLength, pBuffer, cbCurrentLength);
	//
	//		pMFMediaBuffer->Unlock();
	//
	//		pMFMediaBuffer->Release();
	//		pMFSample->Release();
	//	}


	AudioManager::GetInstance()->xAudio2_->CreateSourceVoice(&AudioManager::GetInstance()->audioInformation_[index].pSourceVoice_, waveFormat);


	return index;


	
	
	
	//
	//	Audio::GetInstance()->xAudio2_->CreateSourceVoice(&Audio::GetInstance()->audioInformation_[audioIndex].pSourceVoice_, waveFormat);
	//
	//
	//	return audioIndex;
}







//uint32_t Audio::LoadMP3(const WCHAR* fileName) {
//	//効果音系にはMP3は良いけど
//	//ループにはおすすめできないらしい。末端に無音が入るため。
//
//
//
//
//
//	
//
//	
//}





//音声データの開放
//後ろにあるReleaseで使っているよ
void AudioManager::SoundUnload(uint32_t soundDataHandle) {
	//バッファのメモリを解放
	delete[] AudioManager::GetInstance()->audioInformation_[soundDataHandle].soundData_.pBuffer;
	AudioManager::GetInstance()->audioInformation_[soundDataHandle].soundData_.pBuffer = 0;
	AudioManager::GetInstance()->audioInformation_[soundDataHandle].soundData_.bufferSize = 0;
	AudioManager::GetInstance()->audioInformation_[soundDataHandle].soundData_.wfex = {};

}

//解放
void AudioManager::Release() {
	//解放
	for (int i = 0; i < SOUND_DATE_MAX_; i++) {
		//中身が入っていたらしっかり解放
		if (audioInformation_[i].pSourceReader_ != nullptr) {
			audioInformation_[i].pSourceReader_->Release();
		}
		if (audioInformation_[i].pSourceVoice_ != nullptr) {
			audioInformation_[i].pSourceVoice_->DestroyVoice();
		}
	}


	//XAudio2解放
	xAudio2_.Reset();

	for (int i = 0; i < SOUND_DATE_MAX_; i++) {
		SoundUnload(i);
	}


	MFShutdown();
}


