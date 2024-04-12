#pragma once
#include <iostream>
#include <variant>
#include <string>
#include <Vector3.h>
#include <map>
#include <fstream>

#include <json.hpp>

//項目
struct Item {
	//項目の値
	//variantは複数種類の型のデータを入れることが出来る万能なもの
	//欲しいものを追加していこう！
	std::variant<int32_t,float, Vector3 >value;
};

//グループ
struct Group {
	//<キー,値>
	std::map<std::string, Item>items;
};

//調整項目クラス
//JSONを使うよ
class AdjustmentItems{
private:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	AdjustmentItems() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~AdjustmentItems() = default;



public:
	static AdjustmentItems* GetInstance();

	//コピーコンストラクタ禁止
	AdjustmentItems(const AdjustmentItems& obj) = delete;

	//代入演算子を無効にする
	AdjustmentItems& operator=(const AdjustmentItems& obj) = delete;




public:
	/// <summary>
	/// グループの作成
	/// </summary>
	/// <param name="groupName">グループ名</param>
	void CreateGroup(const std::string& groupName);

#pragma region 値のセット
	//関数テンプレートでまとめても◎


	/// <summary>
	/// 値のセット(int)
	/// </summary>
	/// <param name="groupName">グループ名</param>
	/// <param name="key"></param>
	/// <param name="value">値</param>
	void SetValue(const std::string& groupName, const std::string& key, int32_t value);

	/// <summary>
	/// 値のセット(float)
	/// </summary>
	/// <param name="groupName">グループ名</param>
	/// <param name="key"></param>
	/// <param name="value">値</param>
	void SetValue(const std::string& groupName, const std::string& key, float value);

	/// <summary>
	/// 値のセット(Vector3)
	/// </summary>
	/// <param name="groupName">グループ名</param>
	/// <param name="key"></param>
	/// <param name="value">値</param>
	void SetValue(const std::string& groupName, const std::string& key, const Vector3 value);


#pragma endregion

#pragma region 項目の追加

	//項目が未登録の場合は追加する
	//項目が登録済みの場合は何もしない
	//これによりファイルの読み込みですでに項目が追加された後に
	//初期化処理で値が上書きされないようにする

	/// <summary>
	///	項目の追加(int)
	/// </summary>
	/// <param name="groupName">グループ名</param>
	/// <param name="key"></param>
	/// <param name="value">値</param>
	void AddItem(const std::string& groupName, const std::string& key, int32_t value);

	/// <summary>
	/// 項目の追加(float)
	/// </summary>
	/// <param name="groupName">グループ名</param>
	/// <param name="key"></param>
	/// <param name="value">値</param>
	void AddItem(const std::string& groupName, const std::string& key, float value);


	/// <summary>
	/// 項目の追加(Vector3)
	/// </summary>
	/// <param name="groupName">グループ名</param>
	/// <param name="key"></param>
	/// <param name="value">値</param>
	void AddItem(const std::string& groupName, const std::string& key, const Vector3 value);




#pragma endregion


#pragma region 値の取得
	int32_t GetIntValue(const std::string groupName, const std::string& key);

	float GetFloatValue(const std::string groupName, const std::string& key);

	Vector3 GetVector3Value(const std::string groupName, const std::string& key);

#pragma endregion



	/// <summary>
	/// ファイルに書き出し
	/// </summary>
	/// <param name="groupName">グループ名</param>
	void SaveFile(const std::string& groupName);

	/// <summary>
	/// ディレクトリの「全ファイル」の読み込み
	/// </summary>
	void LoadFile();

	/// <summary>
	/// ファイルから読み込む
	/// </summary>
	/// <param name="groupName">グループ</param>
	void LoadFile(const std::string& groupName);


	/// <summary>
	/// 更新
	/// </summary>
	void Update();


private:
	//全データ
	std::map<std::string, Group>datas_;

	//新しい名前=元の名前
	//冗長だと感じたらusingで短く！
	using json = nlohmann::json;

	//グローバル変数の保存先ファイルパス
	const std::string DIRECTORY_PATH_ = "Resources/JsonFile/";

};

