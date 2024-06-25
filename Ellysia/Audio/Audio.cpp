#include "Audio.h"


#include <imgui.h>

#include "AudioManager.h"
#include <AudioScale.h>



#pragma region 実際に使う関数

#pragma region 基本セット
//音声再生
void Audio::PlayWave(uint32_t audioHandle, bool isLoop) {

	AudioInformation audioInformation = AudioManager::GetInstance()->GetAudioInformation(audioHandle);

	HRESULT hr{};
	hr = audioInformation.pSourceVoice_->FlushSourceBuffers();
	assert(SUCCEEDED(hr));
	//再生する波形データの設定
	XAUDIO2_BUFFER buffer{};
	buffer.pAudioData = audioInformation.soundData_.pBuffer;
	buffer.AudioBytes = audioInformation.soundData_.bufferSize;
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	if (isLoop == true) {
		//ずっとループさせたいならLoopCountにXAUDIO2_LOOP_INFINITEをいれよう
		buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
	}
	else {
		buffer.LoopCount = XAUDIO2_NO_LOOP_REGION;
	}
	//Buffer登録
	hr = audioInformation.pSourceVoice_->SubmitSourceBuffer(&buffer);
	//波形データの再生
	hr = audioInformation.pSourceVoice_->Start(0);



	assert(SUCCEEDED(hr));


}

//ループ回数設定版
void Audio::PlayWave(uint32_t audioHandle, int32_t loopCount) {
	HRESULT hr{};
	AudioInformation audioInformation = AudioManager::GetInstance()->GetAudioInformation(audioHandle);
	hr = audioInformation.pSourceVoice_->FlushSourceBuffers();
	assert(SUCCEEDED(hr));

	//再生する波形データの設定
	XAUDIO2_BUFFER buffer{};
	buffer.pAudioData = audioInformation.soundData_.pBuffer;
	buffer.AudioBytes = audioInformation.soundData_.bufferSize;
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	//ここでループ回数を設定
	//1回多くなっているので-1してあげた方が良いかも
	//1でfalseの場合と同じ
	buffer.LoopCount = loopCount - 1;

	//Buffer登録
	hr = audioInformation.pSourceVoice_->SubmitSourceBuffer(&buffer);
	//波形データの再生
	hr = audioInformation.pSourceVoice_->Start(0);



	assert(SUCCEEDED(hr));
}

void Audio::PlayMP3(uint32_t audioHandle,bool isLoop){
	//MP3はループしない方が良いとのこと
	//一応用意するけど使わないかも
	HRESULT hr{};
	auto audioInformation = AudioManager::GetInstance()->GetAudioInformation(audioHandle);
	hr = audioInformation.pSourceVoice_->FlushSourceBuffers();
	assert(SUCCEEDED(hr));

	XAUDIO2_BUFFER buffer{};
	buffer.pAudioData = audioInformation.mediaData.data();
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.AudioBytes = sizeof(BYTE) * static_cast<UINT32>(audioInformation.mediaData.size());
	if (isLoop == true) {
		//ずっとループさせたいならLoopCountにXAUDIO2_LOOP_INFINITEをいれよう
		buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
	}
	else{
		buffer.LoopCount = XAUDIO2_NO_LOOP_REGION;
	}


	hr = audioInformation.pSourceVoice_->SubmitSourceBuffer(&buffer);

	//波形データの再生
	hr = audioInformation.pSourceVoice_->Start(0);
	assert(SUCCEEDED(hr));
}


//void Audio::PlayMP3(uint32_t audioHandle, bool isLoop) {
//	//MP3はループしない方が良いとのこと
//	//一応用意するけど使わないかも
//	HRESULT hr{};
//	hr = audioInformation_[audioHandle].pSourceVoice_->FlushSourceBuffers();
//	assert(SUCCEEDED(hr));
//
//	XAUDIO2_BUFFER buffer{};
//	buffer.pAudioData = Audio::GetInstance()->audioInformation_[audioHandle].mediaData.data();
//	buffer.Flags = XAUDIO2_END_OF_STREAM;
//	buffer.AudioBytes = sizeof(BYTE) * static_cast<UINT32>(Audio::GetInstance()->audioInformation_[audioHandle].mediaData.size());
//	if (isLoop == true) {
//		//ずっとループさせたいならLoopCountにXAUDIO2_LOOP_INFINITEをいれよう
//		buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
//	}
//	else {
//		buffer.LoopCount = XAUDIO2_NO_LOOP_REGION;
//	}
//
//
//	hr = Audio::GetInstance()->audioInformation_[audioHandle].pSourceVoice_->SubmitSourceBuffer(&buffer);
//
//	//波形データの再生
//	hr = audioInformation_[audioHandle].pSourceVoice_->Start(0);
//	assert(SUCCEEDED(hr));
//}


void Audio::PlayMP3(uint32_t audioHandle, uint32_t loopCount){
	//MP3はループしない方が良いとのこと
	//一応用意するけど使わないかも

	HRESULT hr{};
	AudioInformation audioInformation = AudioManager::GetInstance()->GetAudioInformation(audioHandle);
	hr = audioInformation.pSourceVoice_->FlushSourceBuffers();
	assert(SUCCEEDED(hr));
	XAUDIO2_BUFFER buffer{};
	buffer.pAudioData = audioInformation.mediaData.data();
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.AudioBytes = sizeof(BYTE) * static_cast<UINT32>(audioInformation.mediaData.size());
	//ここでループ回数を設定
	//1回多くなっているので-1してあげた方が良いかも
	//1でfalseの場合と同じ
	buffer.LoopCount = loopCount - 1;



	hr =audioInformation.pSourceVoice_->SubmitSourceBuffer(&buffer);

	//波形データの再生
	hr = audioInformation.pSourceVoice_->Start(0);
	assert(SUCCEEDED(hr));
}

void Audio::PauseWave(uint32_t audioHandle) {
	HRESULT hr{};
	//いきなり停止させて残響とかのエフェクトも停止させたら違和感ある
	//だからXAUDIO2_PLAY_TAILSを入れて余韻も残す
	AudioInformation audioInformation = AudioManager::GetInstance()->GetAudioInformation(audioHandle);
	hr = audioInformation.pSourceVoice_->Stop(XAUDIO2_PLAY_TAILS);
	assert(SUCCEEDED(hr));
}

void Audio::ResumeWave(uint32_t audioHandle) {
	//とはいってもただ再生しているだけ
	//PlayWaveのbufferが無い版

	HRESULT hr{};
	//波形データの再生
	AudioInformation audioInformation = AudioManager::GetInstance()->GetAudioInformation(audioHandle);
	hr = audioInformation.pSourceVoice_->Start(0);
	assert(SUCCEEDED(hr));
}

//音声停止
void Audio::StopWave(uint32_t audioHandle) {
	HRESULT hr{};
	AudioInformation audioInformation = AudioManager::GetInstance()->GetAudioInformation(audioHandle);
	hr = audioInformation.pSourceVoice_->Stop();
	assert(SUCCEEDED(hr));
}

#pragma endregion


#pragma region ループ
void Audio::ExitLoop(uint32_t audioHandle) {
	HRESULT hr{};
	//ExitLoop関数でループを抜ける
	AudioInformation audioInformation = AudioManager::GetInstance()->GetAudioInformation(audioHandle);
	hr = audioInformation.pSourceVoice_->ExitLoop();
	assert(SUCCEEDED(hr));
}


void Audio::AfterLoopPlayWave(uint32_t audioHandle, float second) {
	//別名サスティンループというらしい
	//シンセとかにあるサスティンと関係があるのかな

	//後半ループするよ
	//再生する波形データの設定
	XAUDIO2_BUFFER buffer{};
	AudioInformation audioInformation = AudioManager::GetInstance()->GetAudioInformation(audioHandle);
	buffer.pAudioData = audioInformation.soundData_.pBuffer;
	buffer.AudioBytes = audioInformation.soundData_.bufferSize;
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	//ここでループ回数を設定
	buffer.LoopCount = XAUDIO2_LOOP_INFINITE;

	//長いので新しく変数を作って分かりやすくする
	int samplingRate = audioInformation.soundData_.wfex.nSamplesPerSec;

	//ここでループしたい位置を設定してあげる
	buffer.LoopBegin = uint32_t(second * samplingRate);



	HRESULT hr{};
	//Buffer登録
	hr = audioInformation.pSourceVoice_->SubmitSourceBuffer(&buffer);
	//波形データの再生
	hr = audioInformation.pSourceVoice_->Start(0);



	assert(SUCCEEDED(hr));

}


void Audio::BeforeLoopPlayWave(uint32_t audioHandle, float lengthSecond) {
	//別名サスティンループというらしい
	//シンセとかにあるサスティンと関係があるのかな
	//こっちは前半でループ

	//再生する波形データの設定
	XAUDIO2_BUFFER buffer{};
	AudioInformation audioInformation = AudioManager::GetInstance()->GetAudioInformation(audioHandle);
	buffer.pAudioData = audioInformation.soundData_.pBuffer;
	buffer.AudioBytes = audioInformation.soundData_.bufferSize;
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	//ここでループ回数を設定
	buffer.LoopCount = XAUDIO2_LOOP_INFINITE;

	//長いので新しく変数を作って分かりやすくする
	int samplingRate = audioInformation.soundData_.wfex.nSamplesPerSec;

	//ここでループしたい位置を設定してあげる
	//ここfloatにしたいけど元々がuint32だから無理そう
	buffer.LoopBegin = 0;
	buffer.LoopLength = uint32_t(lengthSecond * samplingRate);



	HRESULT hr{};
	//Buffer登録
	hr = audioInformation.pSourceVoice_->SubmitSourceBuffer(&buffer);
	assert(SUCCEEDED(hr));
	//波形データの再生
	hr = audioInformation.pSourceVoice_->Start(0);



	assert(SUCCEEDED(hr));

}


void Audio::PartlyLoopPlayWave(uint32_t audioHandle, float start, float lengthSecond) {
	//別名サスティンループというらしい
	//シンセとかにあるサスティンと関係があるのかな
	//この関数は部分ループ

	//再生する波形データの設定
	XAUDIO2_BUFFER buffer{};
	AudioInformation audioInformation = AudioManager::GetInstance()->GetAudioInformation(audioHandle);
	buffer.pAudioData = audioInformation.soundData_.pBuffer;
	buffer.AudioBytes = audioInformation.soundData_.bufferSize;
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	//ここでループ回数を設定
	buffer.LoopCount = XAUDIO2_LOOP_INFINITE;

	//長いので新しく変数を作って分かりやすくする
	int samplingRate = audioInformation.soundData_.wfex.nSamplesPerSec;

	//ここでループしたい位置を設定してあげる
	//ここfloatにしたいけど元々がuint32だから無理そう
	buffer.LoopBegin = uint32_t(start * samplingRate);;
	buffer.LoopLength = uint32_t(lengthSecond * samplingRate);



	HRESULT hr{};
	//Buffer登録
	hr = audioInformation.pSourceVoice_->SubmitSourceBuffer(&buffer);
	//波形データの再生
	hr = audioInformation.pSourceVoice_->Start(0);



	assert(SUCCEEDED(hr));
}

#pragma endregion

//一応マイナスにも出来るらしい
//位相の反転するために使うらしい。使い道は分からない。
//音量を変える
void Audio::ChangeVolume(uint32_t audioHandle, float volume) {

	HRESULT hr = {};
	AudioInformation audioInformation = AudioManager::GetInstance()->GetAudioInformation(audioHandle);
	hr = audioInformation.pSourceVoice_->SetVolume(volume);
	assert(SUCCEEDED(hr));
}

//ピッチの変更(滑らか)
void Audio::ChangeFrequency(uint32_t audioHandle, float ratio) {
	HRESULT hr{};

	//これより上がらなかった
	ratio = max(ratio, 2.0f);

	//これより下がらなかった
	ratio = min(ratio, 0.0f);

	AudioInformation audioInformation = AudioManager::GetInstance()->GetAudioInformation(audioHandle);
	hr = audioInformation.pSourceVoice_->SetFrequencyRatio(ratio);
	assert(SUCCEEDED(hr));
}

//ピッチの変更
//シンセとかのように段階的に出来るよ
void Audio::ChangePitch(uint32_t audioHandle, int32_t scale) {

	HRESULT hr{};
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

	AudioInformation audioInformation = AudioManager::GetInstance()->GetAudioInformation(audioHandle);
	hr = audioInformation.pSourceVoice_->SetFrequencyRatio(ratio);
	assert(SUCCEEDED(hr));
}

void Audio::StretchAndPitch(uint32_t audioHandle, float timeRatio, float pitchRatio){
	// タイムストレッチとピッチ調整を行うパラメータを設定する
	XAUDIO2_VOICE_STATE state;
	AudioInformation audioInformation = AudioManager::GetInstance()->GetAudioInformation(audioHandle);
	audioInformation.pSourceVoice_->GetState(&state);
	audioInformation.pSourceVoice_->Stop(0);
	audioInformation.pSourceVoice_->FlushSourceBuffers();

	// タイムストレッチとピッチ調整を行う
	audioInformation.pSourceVoice_->SetFrequencyRatio(timeRatio * pitchRatio);

	// 再生を再開する
	audioInformation.pSourceVoice_->Start(0);
}

//Pan振り
void Audio::SetPan(uint32_t audioHandle, float_t pan) {

	//左右のスピーカー間の目的のパンに基づき送信レベルを計算
	DWORD dwChannelMask = AudioManager::GetInstance()->GetDwChannelMask();
	AudioInformation audioInformation = AudioManager::GetInstance()->GetAudioInformation(audioHandle);
	audioInformation.left_ = 0.5f - pan / 2.0f;
	audioInformation.right_ = 0.5f + pan / 2.0f;
	switch (dwChannelMask)
	{
	case SPEAKER_MONO:
		audioInformation.outputMatrix_[0] = 1.0f;
		break;
	case SPEAKER_STEREO:
	case SPEAKER_2POINT1:
	case SPEAKER_SURROUND:
		audioInformation.outputMatrix_[1] = audioInformation.left_;
		audioInformation.outputMatrix_[2] = audioInformation.right_;

		break;
	case SPEAKER_QUAD:
		audioInformation.outputMatrix_[0] = audioInformation.left_;
		audioInformation.outputMatrix_[1] = audioInformation.right_;
		audioInformation.outputMatrix_[2] = audioInformation.left_;
		audioInformation.outputMatrix_[3] = audioInformation.right_;
		break;
	case SPEAKER_4POINT1:
		audioInformation.outputMatrix_[0] = audioInformation.left_;
		audioInformation.outputMatrix_[1] = audioInformation.right_;
		audioInformation.outputMatrix_[3] = audioInformation.left_;
		audioInformation.outputMatrix_[4] = audioInformation.right_;
		break;
	case SPEAKER_5POINT1:
	case SPEAKER_7POINT1:
	case SPEAKER_5POINT1_SURROUND:
		audioInformation.outputMatrix_[0] = audioInformation.left_;
		audioInformation.outputMatrix_[1] = audioInformation.right_;
		audioInformation.outputMatrix_[4] = audioInformation.left_;
		audioInformation.outputMatrix_[5] = audioInformation.right_;
		break;
	case SPEAKER_7POINT1_SURROUND:
		audioInformation.outputMatrix_[0] = audioInformation.left_;
		audioInformation.outputMatrix_[1] = audioInformation.right_;
		audioInformation.outputMatrix_[4] = audioInformation.left_;
		audioInformation.outputMatrix_[5] = audioInformation.right_;
		audioInformation.outputMatrix_[6] = audioInformation.left_;
		audioInformation.outputMatrix_[7] = audioInformation.right_;
		break;
	}


#pragma region 解説
	//調べても良く分からなかったのでChatGPTに聞いた
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

	//普通のスピーカーは1,2を使う。

#pragma endregion

	XAUDIO2_VOICE_DETAILS voiceDetails;
	
	audioInformation.pSourceVoice_->GetVoiceDetails(&voiceDetails);

	XAUDIO2_VOICE_DETAILS masterVoiiceDetails;
	IXAudio2MasteringVoice* masterVoice = AudioManager::GetInstance()->GetMasterVoice();
	masterVoice->GetVoiceDetails(&masterVoiiceDetails);

	audioInformation.pSourceVoice_->SetOutputMatrix(
		NULL, voiceDetails.InputChannels,
		masterVoiiceDetails.InputChannels,
		audioInformation.outputMatrix_);

}

void Audio::SetLowPassFilter(uint32_t audioHandle, float cutOff) {
	//いきなり効果アリにすると違和感あるよね
	//LowPassは最初「1.0f」にした方が良いかも
	float newCutOff = cutOff;
	if (cutOff > 1.0f) {
		newCutOff = 1.0f;
	}
	else if(cutOff < 0.0f){
		newCutOff = 0.0f;
	}

	//このままだとは7500Hzから下しか掛けられないらしい
	XAUDIO2_FILTER_PARAMETERS FilterParams; //フィルタ指示構造体
	FilterParams.Type = XAUDIO2_FILTER_TYPE::LowPassFilter;
	FilterParams.Frequency = newCutOff;
	FilterParams.OneOverQ = 1.4142f;
	AudioInformation audioInformation = AudioManager::GetInstance()->GetAudioInformation(audioHandle);
	audioInformation.pSourceVoice_->SetFilterParameters(&FilterParams);
}

void Audio::SetLowPassFilter(uint32_t audioHandle, float cutOff, float oneOverQ){
	//いきなり効果アリにすると違和感あるよね
	//LowPassは最初「1.0f」にした方が良いかも
	float newCutOff = cutOff;
	if (cutOff > 1.0f) {
		newCutOff = 1.0f;
	}
	else if (cutOff < 0.0f) {
		newCutOff = 0.0f;
	}

	//このままだとは7500Hzから下しか掛けられないらしい
	XAUDIO2_FILTER_PARAMETERS FilterParams; //フィルタ指示構造体
	FilterParams.Type = XAUDIO2_FILTER_TYPE::LowPassFilter;
	FilterParams.Frequency = newCutOff;
	FilterParams.OneOverQ = oneOverQ;

	AudioInformation audioInformation = AudioManager::GetInstance()->GetAudioInformation(audioHandle);
	audioInformation.pSourceVoice_->SetFilterParameters(&FilterParams);
}

void Audio::SetHighPassFilter(uint32_t audioHandle, float cutOff){
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

	AudioInformation audioInformation = AudioManager::GetInstance()->GetAudioInformation(audioHandle);
	audioInformation.pSourceVoice_->SetFilterParameters(&FilterParams);
}

void Audio::SetHighPassFilter(uint32_t audioHandle, float cutOff, float oneOverQ){
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
	FilterParams.OneOverQ = oneOverQ;

	AudioInformation audioInformation = AudioManager::GetInstance()->GetAudioInformation(audioHandle);
	audioInformation.pSourceVoice_->SetFilterParameters(&FilterParams);
}

void Audio::SetBandPassFilter(uint32_t audioHandle, float cutOff){
	//いきなり効果アリにすると違和感あるよね

	float newCutOff = cutOff;
	if (cutOff > 1.0f) {
		newCutOff = 1.0f;
	}
	else if (cutOff < 0.0f) {
		newCutOff = 0.0f;
	}

	XAUDIO2_FILTER_PARAMETERS FilterParams;
	FilterParams.Type = XAUDIO2_FILTER_TYPE::BandPassFilter;
	//0.5f
	FilterParams.Frequency = newCutOff;
	FilterParams.OneOverQ = 1.0f;
	AudioInformation audioInformation = AudioManager::GetInstance()->GetAudioInformation(audioHandle);
	audioInformation.pSourceVoice_->SetFilterParameters(&FilterParams);
}

void Audio::SetBandPassFilter(uint32_t audioHandle, float cutOff, float oneOverQ){
	//いきなり効果アリにすると違和感あるよね

	float newCutOff = cutOff;
	if (cutOff > 1.0f) {
		newCutOff = 1.0f;
	}
	else if (cutOff < 0.0f) {
		newCutOff = 0.0f;
	}

	XAUDIO2_FILTER_PARAMETERS FilterParams;
	FilterParams.Type = XAUDIO2_FILTER_TYPE::NotchFilter;;
	//0.5f
	FilterParams.Frequency = newCutOff;
	FilterParams.OneOverQ = oneOverQ;

	AudioInformation audioInformation = AudioManager::GetInstance()->GetAudioInformation(audioHandle);
	audioInformation.pSourceVoice_->SetFilterParameters(&FilterParams);
}

void Audio::SetNotchFilter(uint32_t audioHandle, float cutOff){
	//いきなり効果アリにすると違和感あるよね

	float newCutOff = cutOff;
	if (cutOff > 1.0f) {
		newCutOff = 1.0f;
	}
	else if (cutOff < 0.0f) {
		newCutOff = 0.0f;
	}

	XAUDIO2_FILTER_PARAMETERS FilterParams;
	FilterParams.Type = XAUDIO2_FILTER_TYPE::BandPassFilter;
	//0.5f
	FilterParams.Frequency = newCutOff;
	FilterParams.OneOverQ = 1.0f;
	AudioInformation audioInformation = AudioManager::GetInstance()->GetAudioInformation(audioHandle);
	audioInformation.pSourceVoice_->SetFilterParameters(&FilterParams);
}

void Audio::SetNotchFilter(uint32_t audioHandle, float cutOff, float oneOverQ){
	//いきなり効果アリにすると違和感あるよね

	float newCutOff = cutOff;
	if (cutOff > 1.0f) {
		newCutOff = 1.0f;
	}
	else if (cutOff < 0.0f) {
		newCutOff = 0.0f;
	}

	XAUDIO2_FILTER_PARAMETERS FilterParams;
	FilterParams.Type = XAUDIO2_FILTER_TYPE::BandPassFilter;
	//0.5f
	FilterParams.Frequency = newCutOff;
	FilterParams.OneOverQ = oneOverQ;

	AudioInformation audioInformation = AudioManager::GetInstance()->GetAudioInformation(audioHandle);
	audioInformation.pSourceVoice_->SetFilterParameters(&FilterParams);
}



void Audio::SendChannels(uint32_t audioHandle,uint32_t channelNumber){
	IXAudio2SubmixVoice* subMixVoice = AudioManager::GetInstance()->GetSubmixVoice(0);
	
	XAUDIO2_SEND_DESCRIPTOR send = { 0, subMixVoice };
	XAUDIO2_VOICE_SENDS sendlist = { channelNumber, &send };

	HRESULT hr = {};
	AudioInformation audioInformation = AudioManager::GetInstance()->GetAudioInformation(audioHandle);
	audioInformation.pSourceVoice_->SetOutputVoices(&sendlist);
	assert(SUCCEEDED(hr));
}

void Audio::CreateReverb(uint32_t audioHandle,uint32_t channel) {
	audioHandle;
	channel;
	//XAudio2CreateReverb(&pXAPO_);

	//// EFFECT_DESCRIPTOR の作成
	//XAUDIO2_EFFECT_DESCRIPTOR descriptor;
	//descriptor.InitialState = true; // 最初から有効状態
	//descriptor.OutputChannels = 2; // 2ch のエフェクトだよ
	//descriptor.pEffect = pXAPO_; // エフェクト本体
	//// EFFECT_CHAIN の作成
	//XAUDIO2_EFFECT_CHAIN chain;
	//chain.EffectCount = 1; // 挿すのは 1 個ですよ
	//chain.pEffectDescriptors = &descriptor; // さっきの構造体を指示
	//// ボイスに EFFECT_CHAIN を挿す
	//audioInformation_[audioHandle].pSourceVoice_->SetEffectChain(&chain);

	//// EFFECT_CHAIN の作成
	//XAUDIO2_EFFECT_CHAIN chain;
	//chain.EffectCount = 1;
	//chain.pEffectDescriptors = &descriptor;

	//// ボイスに EFFECT_CHAIN を挿す
	//Audio::GetInstance()->submixVoice_[channel]->SetEffectChain(&chain);

	//pXAPO_->Release();
}


//エフェクトの効果を無効にする
void Audio::OffEffect(uint32_t audioHandle) {
	HRESULT hr{};
	AudioInformation audioInformation = AudioManager::GetInstance()->GetAudioInformation(audioHandle);
	hr = audioInformation.pSourceVoice_->DisableEffect(0);
	assert(SUCCEEDED(hr));
}

//エフェクトの効果を有効にする
void Audio::OnEffect(uint32_t audioHandle) {
	HRESULT hr{};
	AudioInformation audioInformation = AudioManager::GetInstance()->GetAudioInformation(audioHandle);
	hr = audioInformation.pSourceVoice_->EnableEffect(0);
	assert(SUCCEEDED(hr));
}



#pragma endregion
