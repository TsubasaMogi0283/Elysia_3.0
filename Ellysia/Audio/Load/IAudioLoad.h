#pragma once
#include <string>

//このクラスをもとにオーディオの読み込みクラスを作る

class IAudioLoad {
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	IAudioLoad() = default;

	/// <summary>
	/// 読み込み関数
	/// </summary>
	/// <param name="fileName"></param>
	virtual void Load(const std::string& fileName)=0;






};

