#include "Audio.h"
#include <ConvertLog.h>








Audio* Audio::GetInstance() {
	static Audio instance;
	return &instance;
}

void Audio::CreateSubmixVoice(uint32_t channel) {
	uint32_t channels = channel;
	uint32_t sampleRate = 44100;



	HRESULT hr = {};
	hr = Audio::GetInstance()->xAudio2_->CreateSubmixVoice(&Audio::GetInstance()->submixVoice_[channel], channels, sampleRate);
	assert(SUCCEEDED(hr));

}

//初期化
//これはDirecX初期化の後に入れてね
void Audio::Initialize() {
	// Media Foundation の初期化
	HRESULT hr = MFStartup(MF_VERSION);
	assert(SUCCEEDED(hr));

	//XAudioのエンジンのインスタンスを生成
	hr = XAudio2Create(&xAudio2_, 0, XAUDIO2_DEFAULT_PROCESSOR);
	assert(SUCCEEDED(hr));

	//マスターボイスを生成
	hr = xAudio2_->CreateMasteringVoice(&masterVoice_);
	assert(SUCCEEDED(hr));

	//スピーカ構成を取得
	masterVoice_->GetChannelMask(&dwChannelMask_);

	//一度全部0に初期化
	for (int i = 0; i < 8; i++) {
		outputMatrix_[i] = 0;
	}

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
uint32_t Audio::LoadWave(const std::string& fileName) {

	//16,24,32bitは読み込み出来た
	//64bitも読み込み出来るようにしたいと思ったが一般的に使われないらしい
	//だから32が最大で良いかも。
	//64bitを書き出せるCakewslkすご



	//mapにしたい
	//一度読み込んだものは２度読み込まず返すだけ
	for (int i = 0; i < SOUND_DATE_MAX_; i++) {
		if (Audio::GetInstance()->audioInformation_[i].fileName == fileName) {
			return Audio::GetInstance()->audioInformation_[i].handle;
		}
	}


	//indexを取得
	uint32_t handle = Audio::GetInstance()->index_;
	//加算
	Audio::GetInstance()->index_++;




	//記録
	Audio::GetInstance()->audioInformation_[handle].fileName = fileName;
	Audio::GetInstance()->audioInformation_[handle].handle = handle;


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

	Audio::GetInstance()->audioInformation_[handle].soundData.wfex = format.fmt;
	Audio::GetInstance()->audioInformation_[handle].soundData.pBuffer = reinterpret_cast<BYTE*>(pBuffer);
	Audio::GetInstance()->audioInformation_[handle].soundData.bufferSize = data.size;


	//波形フォーマットを基にSourceVoiceの生成
	HRESULT hr{};
	hr = Audio::GetInstance()->xAudio2_->CreateSourceVoice(
		&Audio::GetInstance()->audioInformation_[handle].sourceVoice,
		&Audio::GetInstance()->audioInformation_[handle].soundData.wfex);
	assert(SUCCEEDED(hr));


	return handle;

#pragma endregion



}




uint32_t Audio::LoadMP3(const std::string& fileName) {


	//一度読み込んだものは２度読み込まず返すだけ
	for (int i = 0; i < SOUND_DATE_MAX_; i++) {
		if (Audio::GetInstance()->audioInformation_[i].fileName == fileName) {
			return Audio::GetInstance()->audioInformation_[i].handle;
		}
	}

	Audio::GetInstance()->index_++;

	//記録
	Audio::GetInstance()->audioInformation_[Audio::GetInstance()->index_].fileName = fileName;
	Audio::GetInstance()->audioInformation_[Audio::GetInstance()->index_].handle = Audio::GetInstance()->index_;


	//stringからLPCWCHARに変換する
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, fileName.c_str(), (int)fileName.size(), NULL, 0);
	std::wstring wstr(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, fileName.c_str(), (int)fileName.size(), &wstr[0], size_needed);
	LPCWSTR lpcWString = wstr.c_str();


	//ソースリーダーの作成
	HRESULT hResult = MFCreateSourceReaderFromURL(lpcWString, nullptr, &Audio::GetInstance()->audioInformation_[Audio::GetInstance()->index_].sourceReader);
	assert(SUCCEEDED(hResult));



	//メディアタイプの取得
	IMFMediaType* pMFMediaType{ nullptr };
	MFCreateMediaType(&pMFMediaType);
	pMFMediaType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio);
	pMFMediaType->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM);
	Audio::GetInstance()->audioInformation_[Audio::GetInstance()->index_].sourceReader->SetCurrentMediaType(DWORD(MF_SOURCE_READER_FIRST_AUDIO_STREAM), nullptr, pMFMediaType);

	pMFMediaType->Release();
	pMFMediaType = nullptr;
	Audio::GetInstance()->audioInformation_[Audio::GetInstance()->index_].sourceReader->GetCurrentMediaType(DWORD(MF_SOURCE_READER_FIRST_AUDIO_STREAM), &pMFMediaType);

	//オーディオデータ形式の作成
	WAVEFORMATEX* waveFormat{};
	MFCreateWaveFormatExFromMFMediaType(pMFMediaType, &waveFormat, nullptr);



	while (true)
	{
		IMFSample* pMFSample{ nullptr };
		DWORD dwStreamFlags{ 0 };
		Audio::GetInstance()->audioInformation_[Audio::GetInstance()->index_].sourceReader->ReadSample(DWORD(MF_SOURCE_READER_FIRST_AUDIO_STREAM), 0, nullptr, &dwStreamFlags, nullptr, &pMFSample);

		if (dwStreamFlags & MF_SOURCE_READERF_ENDOFSTREAM)
		{
			break;
		}

		IMFMediaBuffer* pMFMediaBuffer{ nullptr };
		pMFSample->ConvertToContiguousBuffer(&pMFMediaBuffer);

		BYTE* pBuffer{ nullptr };
		DWORD cbCurrentLength{ 0 };
		pMFMediaBuffer->Lock(&pBuffer, nullptr, &cbCurrentLength);

		Audio::GetInstance()->audioInformation_[Audio::GetInstance()->index_].mediaData.resize(Audio::GetInstance()->audioInformation_[Audio::GetInstance()->index_].mediaData.size() + cbCurrentLength);
		memcpy(Audio::GetInstance()->audioInformation_[Audio::GetInstance()->index_].mediaData.data() + Audio::GetInstance()->audioInformation_[Audio::GetInstance()->index_].mediaData.size() - cbCurrentLength, pBuffer, cbCurrentLength);

		pMFMediaBuffer->Unlock();

		pMFMediaBuffer->Release();
		pMFSample->Release();
	}

	Audio::GetInstance()->xAudio2_->CreateSourceVoice(&Audio::GetInstance()->audioInformation_[Audio::GetInstance()->index_].sourceVoice, waveFormat);


	return Audio::GetInstance()->index_;
}


void Audio::PlayMP3(uint32_t audioHandle, bool isLoop) {
	
	HRESULT hResult = audioInformation_[audioHandle].sourceVoice->FlushSourceBuffers();
	assert(SUCCEEDED(hResult));

	//bufferの設定
	XAUDIO2_BUFFER buffer{};
	buffer.pAudioData = Audio::GetInstance()->audioInformation_[audioHandle].mediaData.data();
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.AudioBytes = sizeof(BYTE) * static_cast<UINT32>(Audio::GetInstance()->audioInformation_[audioHandle].mediaData.size());
	if (isLoop == true) {
		//ずっとループさせたいならLoopCountにXAUDIO2_LOOP_INFINITEをいれよう
		buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
	}
	else {
		buffer.LoopCount = XAUDIO2_NO_LOOP_REGION;
	}


	hResult = Audio::GetInstance()->audioInformation_[audioHandle].sourceVoice->SubmitSourceBuffer(&buffer);
	assert(SUCCEEDED(hResult));

	//波形データの再生
	hResult = audioInformation_[audioHandle].sourceVoice->Start(0);
	assert(SUCCEEDED(hResult));
}

void Audio::PlayMP3(uint32_t audioHandle, uint32_t loopCount) {
	HRESULT hResult = audioInformation_[audioHandle].sourceVoice->FlushSourceBuffers();
	assert(SUCCEEDED(hResult));

	//bufferの設定
	XAUDIO2_BUFFER buffer{};
	buffer.pAudioData = Audio::GetInstance()->audioInformation_[audioHandle].mediaData.data();
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.AudioBytes = sizeof(BYTE) * static_cast<UINT32>(Audio::GetInstance()->audioInformation_[audioHandle].mediaData.size());
	//ここでループ回数を設定
	//1回多くなっているので-1してあげた方が良いかも
	//1でfalseの場合と同じ
	buffer.LoopCount = loopCount - 1;

	hResult = Audio::GetInstance()->audioInformation_[audioHandle].sourceVoice->SubmitSourceBuffer(&buffer);
	assert(SUCCEEDED(hResult));

	//波形データの再生
	hResult = audioInformation_[audioHandle].sourceVoice->Start(0);
	assert(SUCCEEDED(hResult));
}




//音声再生
void Audio::PlayWave(uint32_t audioHandle, bool isLoop) {
	HRESULT hr{};
	hr = audioInformation_[audioHandle].sourceVoice->FlushSourceBuffers();
	assert(SUCCEEDED(hr));
	//再生する波形データの設定
	XAUDIO2_BUFFER buffer{};
	buffer.pAudioData = audioInformation_[audioHandle].soundData.pBuffer;
	buffer.AudioBytes = audioInformation_[audioHandle].soundData.bufferSize;
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	if (isLoop == true) {
		//ずっとループさせたいならLoopCountにXAUDIO2_LOOP_INFINITEをいれよう
		buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
	}
	if (isLoop == false) {
		buffer.LoopCount = XAUDIO2_NO_LOOP_REGION;
	}


	//Buffer登録
	hr = audioInformation_[audioHandle].sourceVoice->SubmitSourceBuffer(&buffer);
	assert(SUCCEEDED(hr));
	//波形データの再生
	hr = audioInformation_[audioHandle].sourceVoice->Start();



	assert(SUCCEEDED(hr));
}

//ループ回数設定版
void Audio::PlayWave(uint32_t audioHandle, int32_t loopCount) {


	//再生する波形データの設定
	XAUDIO2_BUFFER buffer{};
	buffer.pAudioData = audioInformation_[audioHandle].soundData.pBuffer;
	buffer.AudioBytes = audioInformation_[audioHandle].soundData.bufferSize;
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	//ここでループ回数を設定
	//1回多くなっているので-1してあげた方が良いかも
	buffer.LoopCount = loopCount - 1;


	HRESULT hr{};
	//Buffer登録
	hr = audioInformation_[audioHandle].sourceVoice->SubmitSourceBuffer(&buffer);
	//波形データの再生
	hr = audioInformation_[audioHandle].sourceVoice->Start();



	assert(SUCCEEDED(hr));
}

void Audio::PauseWave(uint32_t audioHandle) {
	//いきなり停止させて残響とかのエフェクトも停止させたら違和感ある
	//だからXAUDIO2_PLAY_TAILSを入れて余韻も残す
	HRESULT hResult = audioInformation_[audioHandle].sourceVoice->Stop(XAUDIO2_PLAY_TAILS);
	assert(SUCCEEDED(hResult));
}

void Audio::ResumeWave(uint32_t audioHandle) {
	//波形データの再生
	HRESULT hResult = audioInformation_[audioHandle].sourceVoice->Start(0);
	assert(SUCCEEDED(hResult));
}

//音声停止
void Audio::StopWave(uint32_t audioHandle) {
	HRESULT hResult = audioInformation_[audioHandle].sourceVoice->Stop();
	assert(SUCCEEDED(hResult));
}

#pragma endregion


#pragma region ループ
void Audio::ExitLoop(uint32_t audioHandle) {
	//ExitLoop関数でループを抜ける
	HRESULT hr = audioInformation_[audioHandle].sourceVoice->ExitLoop();
	assert(SUCCEEDED(hr));
}


void Audio::AfterLoopPlayWave(uint32_t audioHandle, float second) {
	//別名サスティンループというらしい
	//シンセとかにあるサスティンと関係があるのかな

	//後半ループするよ
	//再生する波形データの設定
	XAUDIO2_BUFFER buffer{};
	buffer.pAudioData = audioInformation_[audioHandle].soundData.pBuffer;
	buffer.AudioBytes = audioInformation_[audioHandle].soundData.bufferSize;
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	//ここでループ回数を設定
	buffer.LoopCount = XAUDIO2_LOOP_INFINITE;

	//長いので新しく変数を作って分かりやすくする
	int samplingRate = Audio::GetInstance()->audioInformation_[index_].soundData.wfex.nSamplesPerSec;

	//ここでループしたい位置を設定してあげる
	buffer.LoopBegin = uint32_t(second * samplingRate);



	//Buffer登録
	HRESULT hResult = audioInformation_[audioHandle].sourceVoice->SubmitSourceBuffer(&buffer);
	assert(SUCCEEDED(hResult));

	//波形データの再生
	hResult = audioInformation_[audioHandle].sourceVoice->Start(0);
	assert(SUCCEEDED(hResult));

}


void Audio::BeforeLoopPlayWave(uint32_t audioHandle, float lengthSecond) {
	//別名サスティンループというらしい
	//シンセとかにあるサスティンと関係があるのかな
	//こっちは前半でループ

	//再生する波形データの設定
	XAUDIO2_BUFFER buffer{};
	buffer.pAudioData = audioInformation_[audioHandle].soundData.pBuffer;
	buffer.AudioBytes = audioInformation_[audioHandle].soundData.bufferSize;
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	//ここでループ回数を設定
	buffer.LoopCount = XAUDIO2_LOOP_INFINITE;

	//長いので新しく変数を作って分かりやすくする
	int samplingRate = Audio::GetInstance()->audioInformation_[index_].soundData.wfex.nSamplesPerSec;

	//ここでループしたい位置を設定してあげる
	buffer.LoopBegin = 0;
	buffer.LoopLength = static_cast<uint32_t>(lengthSecond * samplingRate);



	//Buffer登録
	HRESULT hResult = audioInformation_[audioHandle].sourceVoice->SubmitSourceBuffer(&buffer);
	assert(SUCCEEDED(hResult));

	//波形データの再生
	hResult = audioInformation_[audioHandle].sourceVoice->Start(0);
	assert(SUCCEEDED(hResult));

}


void Audio::PartlyLoopPlayWave(uint32_t audioHandle, float start, float lengthSecond) {
	//別名サスティンループというらしい
	//シンセとかにあるサスティンと関係があるのかな
	//こっちは前半でループ

	//再生する波形データの設定
	XAUDIO2_BUFFER buffer{};
	buffer.pAudioData = audioInformation_[audioHandle].soundData.pBuffer;
	buffer.AudioBytes = audioInformation_[audioHandle].soundData.bufferSize;
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	//ここでループ回数を設定
	buffer.LoopCount = XAUDIO2_LOOP_INFINITE;

	//長いので新しく変数を作って分かりやすくする
	int samplingRate = Audio::GetInstance()->audioInformation_[index_].soundData.wfex.nSamplesPerSec;

	//ここでループしたい位置を設定してあげる
	buffer.LoopBegin = static_cast<uint32_t>(start * samplingRate);
	buffer.LoopLength = static_cast<uint32_t>(lengthSecond * samplingRate);



	
	//Buffer登録
	HRESULT hResult = audioInformation_[audioHandle].sourceVoice->SubmitSourceBuffer(&buffer);
	assert(SUCCEEDED(hResult));
	
	//波形データの再生
	hResult = audioInformation_[audioHandle].sourceVoice->Start(0);
	assert(SUCCEEDED(hResult));


	
}

#pragma endregion

//一応マイナスにも出来るらしい
//位相の反転するために使うらしい。使い道は分からない。
//音量を変える
void Audio::ChangeVolume(uint32_t audioHandle, float volume) {

	 HRESULT hResult = audioInformation_[audioHandle].sourceVoice->SetVolume(volume);
	assert(SUCCEEDED(hResult));
}

//ピッチの変更(滑らか)
void Audio::ChangeFrequency(uint32_t audioHandle, float ratio) {

	//2.0fより上がらなかった
	ratio = max(ratio, 2.0f);

	//0.0fより下がらなかった
	ratio = min(ratio, 0.0f);


	HRESULT hResult = audioInformation_[audioHandle].sourceVoice->SetFrequencyRatio(ratio);
	assert(SUCCEEDED(hResult));
}





void Audio::ChangePitch(uint32_t audioHandle, int32_t scale) {

	
	float ratio = 1.0f;

	//入力された値がプラスだった場合
	if (scale >= 0) {
		//プラスのピッチの方を探す
		for (int32_t i = 0; i < SCALE_AMOUNT_; i++) {
			if (scale == i) {
				ratio = SEMITONE_RATIO_[i];
				break;
			}

		}
		//12以上は上がらなかった
		if (scale > 12) {
			ratio = SEMITONE_RATIO_[12];
		}

	}
	//入力された値がマイナスだった場合
	else if (scale < 0) {
		//マイナスのピッチの方を探す
		for (int32_t i = 0; i < SCALE_AMOUNT_; i++) {
			if (scale == -i) {
				ratio = MINUS_SEMITONE_RATION[i];
				break;
			}

		}
		//-12以上は下がらなかった
		if (scale < -12) {
			ratio = MINUS_SEMITONE_RATION[12];
		}
	}

	//比率の設定
	HRESULT hResult = audioInformation_[audioHandle].sourceVoice->SetFrequencyRatio(ratio);
	assert(SUCCEEDED(hResult));
}

//Pan振り
void Audio::SetPan(uint32_t audioHandle, float_t pan) {

	//左右のスピーカー間の目的のパンに基づき送信レベルを計算
	left_ = 0.5f - pan / 2.0f;
	right_ = 0.5f + pan / 2.0f;
	switch (dwChannelMask_)
	{
	case SPEAKER_MONO:
		outputMatrix_[0] = 1.0f;
		break;
	case SPEAKER_STEREO:
	case SPEAKER_2POINT1:
	case SPEAKER_SURROUND:
		outputMatrix_[1] = left_;
		outputMatrix_[2] = right_;

		break;
	case SPEAKER_QUAD:
		outputMatrix_[0] = left_;
		outputMatrix_[1] = right_;
		outputMatrix_[2] = left_;
		outputMatrix_[3] = right_;
		break;
	case SPEAKER_4POINT1:
		outputMatrix_[0] = left_;
		outputMatrix_[1] = right_;
		outputMatrix_[3] = left_;
		outputMatrix_[4] = right_;
		break;
	case SPEAKER_5POINT1:
	case SPEAKER_7POINT1:
	case SPEAKER_5POINT1_SURROUND:
		outputMatrix_[0] = left_;
		outputMatrix_[1] = right_;
		outputMatrix_[4] = left_;
		outputMatrix_[5] = right_;
		break;
	case SPEAKER_7POINT1_SURROUND:
		outputMatrix_[0] = left_;
		outputMatrix_[1] = right_;
		outputMatrix_[4] = left_;
		outputMatrix_[5] = right_;
		outputMatrix_[6] = left_;
		outputMatrix_[7] = right_;
		break;
	}


#pragma region 解説
	//調べてもよく分からなかったのでChatGPTに聞いた
	//outputMatrix_[0]: 主にモノラル音声の場合に使用され、すべての音声を単一のスピーカーに送信します。
	//outputMatrix_[1] : ステレオ音声の場合、左側のスピーカーに対する音声の振幅を指定します。
	//outputMatrix_[2] : ステレオ音声の場合、右側のスピーカーに対する音声の振幅を指定します。
	//outputMatrix_[3] : クアッドフォニックスシステムの場合、左前のスピーカーに対する音声の振幅を指定します。
	//outputMatrix_[4] : クアッドフォニックスシステムの場合、右前のスピーカーに対する音声の振幅を指定します。
	//outputMatrix_[5] : クアッドフォニックスシステムの場合、左後ろのスピーカーに対する音声の振幅を指定します。
	//outputMatrix_[6] : クアッドフォニックスシステムの場合、右後ろのスピーカーに対する音声の振幅を指定します。
	//outputMatrix_[7] : 7.1サラウンドシステムなどのように、サラウンドサウンドをサポートするシステムにおいて、
	//	追加のサラウンドスピーカーに対する音声の振幅を指定するためのものです。
	//	通常、これはセンターバックスピーカーに対する音声の振幅を制御するために使用されます。

	//公式嘘ついてる・・
	//0,1だけだと左しかできないし。
	//要約するとそれぞれのスピーカーの位置的なやつだと。

	//普通のスピーカーは1,2を使う。L,Rだから2つだね。

#pragma endregion

	//詳細の取得
	XAUDIO2_VOICE_DETAILS voiceDetails;
	audioInformation_[audioHandle].sourceVoice->GetVoiceDetails(&voiceDetails);

	//マスターの詳細を取得
	XAUDIO2_VOICE_DETAILS masterVoiiceDetails;
	masterVoice_->GetVoiceDetails(&masterVoiiceDetails);

	//OutPutMatrixに設定
	HRESULT hResult = audioInformation_[audioHandle].sourceVoice->SetOutputMatrix(
		NULL, voiceDetails.InputChannels,
		masterVoiiceDetails.InputChannels,
		outputMatrix_);
	assert(SUCCEEDED(hResult));

}

void Audio::SetLowPassFilter(uint32_t audioHandle, float cutOff) {
	//いきなり効果アリにすると違和感あるよね
	//LowPassは最初「1.0f」にした方が良いかも
	float newCutOff = cutOff;
	if (cutOff > 1.0f) {
		newCutOff = 1.0f;
	}
	else if (cutOff < 0.0f) {
		newCutOff = 0.0f;
	}

	XAUDIO2_FILTER_PARAMETERS FilterParams;
	FilterParams.Type = XAUDIO2_FILTER_TYPE::LowPassFilter;
	FilterParams.Frequency = newCutOff;
	FilterParams.OneOverQ = 1.4142f;

	//パラメータの設定
	HRESULT hResult = audioInformation_[audioHandle].sourceVoice->SetFilterParameters(&FilterParams);
	assert(SUCCEEDED(hResult));

}

void Audio::SetLowPassFilter(uint32_t audioHandle, float cutOff, float oneOverQ) {
	//いきなり効果アリにすると違和感あるよね
	//LowPassは最初「1.0f」にした方が良いかも
	float newCutOff = cutOff;
	if (cutOff > 1.0f) {
		newCutOff = 1.0f;
	}
	else if (cutOff < 0.0f) {
		newCutOff = 0.0f;
	}


	XAUDIO2_FILTER_PARAMETERS FilterParams; 
	FilterParams.Type = XAUDIO2_FILTER_TYPE::LowPassFilter;
	FilterParams.Frequency = newCutOff;
	FilterParams.OneOverQ = oneOverQ;

	//パラメータの設定
	HRESULT hResult = audioInformation_[audioHandle].sourceVoice->SetFilterParameters(&FilterParams);
	assert(SUCCEEDED(hResult));
}

void Audio::SetHighPassFilter(uint32_t audioHandle, float cutOff) {
	//いきなり効果アリにすると違和感あるよね
	//HighPassは最初「0.0f」にした方が良いかも


	float newCutOff = cutOff;
	if (cutOff > 1.0f) {
		newCutOff = 1.0f;
	}
	else if (cutOff < 0.0f) {
		newCutOff = 0.0f;
	}

	XAUDIO2_FILTER_PARAMETERS FilterParams;
	FilterParams.Type = XAUDIO2_FILTER_TYPE::HighPassFilter;
	FilterParams.Frequency = newCutOff;
	FilterParams.OneOverQ = 1.4142f;

	//パラメータの設定
	HRESULT hResult = audioInformation_[audioHandle].sourceVoice->SetFilterParameters(&FilterParams);
	assert(SUCCEEDED(hResult));
}

void Audio::SetHighPassFilter(uint32_t audioHandle, float cutOff, float oneOverQ) {
	
	cutOff = max(cutOff,1.0f);
	cutOff = min(cutOff, 0.0f);


	XAUDIO2_FILTER_PARAMETERS FilterParams;
	FilterParams.Type = XAUDIO2_FILTER_TYPE::HighPassFilter;
	FilterParams.Frequency = cutOff;
	FilterParams.OneOverQ = oneOverQ;

	//パラメータの設定
	HRESULT hResult = audioInformation_[audioHandle].sourceVoice->SetFilterParameters(&FilterParams);
	assert(SUCCEEDED(hResult));
}

void Audio::SetBandPassFilter(uint32_t audioHandle, float cutOff) {
	

	cutOff = max(cutOff, 1.0f);
	cutOff = min(cutOff, 0.0f);


	XAUDIO2_FILTER_PARAMETERS FilterParams;
	FilterParams.Type = XAUDIO2_FILTER_TYPE::BandPassFilter;
	//0.5f
	FilterParams.Frequency = cutOff;
	FilterParams.OneOverQ = 1.0f;
	HRESULT hResult = audioInformation_[audioHandle].sourceVoice->SetFilterParameters(&FilterParams);
	assert(SUCCEEDED(hResult));
}

void Audio::SetBandPassFilter(uint32_t audioHandle, float cutOff, float oneOverQ) {
	
	cutOff = max(cutOff, 1.0f);
	cutOff = min(cutOff, 0.0f);


	XAUDIO2_FILTER_PARAMETERS FilterParams;
	FilterParams.Type = XAUDIO2_FILTER_TYPE::NotchFilter;;
	//0.5f
	FilterParams.Frequency = cutOff;
	FilterParams.OneOverQ = oneOverQ;

	//パラメーターの設定
	HRESULT hResult=audioInformation_[audioHandle].sourceVoice->SetFilterParameters(&FilterParams);
	assert(SUCCEEDED(hResult));
}

void Audio::SetNotchFilter(uint32_t audioHandle, float cutOff) {
	cutOff = max(cutOff, 1.0f);
	cutOff = min(cutOff, 0.0f);


	XAUDIO2_FILTER_PARAMETERS FilterParams;
	FilterParams.Type = XAUDIO2_FILTER_TYPE::BandPassFilter;
	FilterParams.Frequency = cutOff;
	FilterParams.OneOverQ = 1.0f;

	//パラメーターの設定
	HRESULT hResult = audioInformation_[audioHandle].sourceVoice->SetFilterParameters(&FilterParams);
	assert(SUCCEEDED(hResult));
}

void Audio::SetNotchFilter(uint32_t audioHandle, float cutOff, float oneOverQ) {

	cutOff = max(cutOff, 1.0f);
	cutOff = min(cutOff, 0.0f);


	XAUDIO2_FILTER_PARAMETERS FilterParams;
	FilterParams.Type = XAUDIO2_FILTER_TYPE::BandPassFilter;
	FilterParams.Frequency = cutOff;
	FilterParams.OneOverQ = oneOverQ;

	//パラメーターの設定
	HRESULT hResult=audioInformation_[audioHandle].sourceVoice->SetFilterParameters(&FilterParams);
	assert(SUCCEEDED(hResult));
}



void Audio::SendChannels(uint32_t audioHandle, uint32_t channelNumber) {
	XAUDIO2_SEND_DESCRIPTOR send = { 0, Audio::GetInstance()->submixVoice_[0] };
	XAUDIO2_VOICE_SENDS sendlist = { channelNumber, &send };

	HRESULT hResult=audioInformation_[audioHandle].sourceVoice->SetOutputVoices(&sendlist);
	assert(SUCCEEDED(hResult));
}

void Audio::CreateReverb(uint32_t audioHandle, uint32_t channel) {
	audioHandle;
	channel;
}


//エフェクトの効果を無効にする
void Audio::OffEffect(uint32_t audioHandle) {
	HRESULT hResult = audioInformation_[audioHandle].sourceVoice->DisableEffect(0);
	assert(SUCCEEDED(hResult));
}

//エフェクトの効果を有効にする
void Audio::OnEffect(uint32_t audioHandle) {
	HRESULT hResult = audioInformation_[audioHandle].sourceVoice->EnableEffect(0);
	assert(SUCCEEDED(hResult));
}



#pragma endregion


//音声データの開放
//後ろにあるReleaseで使っているよ
void Audio::SoundUnload(uint32_t soundDataHandle) {
	//バッファのメモリを解放
	delete[] Audio::GetInstance()->audioInformation_[soundDataHandle].soundData.pBuffer;
	Audio::GetInstance()->audioInformation_[soundDataHandle].soundData.pBuffer = 0;
	Audio::GetInstance()->audioInformation_[soundDataHandle].soundData.bufferSize = 0;
	Audio::GetInstance()->audioInformation_[soundDataHandle].soundData.wfex = {};

}

//解放
void Audio::Release() {

	//pXAPO_->Release();
	for (int i = 0; i < SOUND_DATE_MAX_; i++) {
		if (audioInformation_[i].sourceVoice != nullptr) {
			audioInformation_[i].sourceVoice->DestroyVoice();
		}
	}

	//XAudio2解放
	xAudio2_.Reset();

	for (int i = 0; i < SOUND_DATE_MAX_; i++) {
		SoundUnload(i);
	}

}

