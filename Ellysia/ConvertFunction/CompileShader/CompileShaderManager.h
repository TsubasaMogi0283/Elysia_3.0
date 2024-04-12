#pragma once
#include <string>

#include <dxgidebug.h>
#include "ConvertLog.h"
#include <cassert>



#include <dxcapi.h>
#pragma comment(lib,"dxcompiler.lib")



//コンパイル関数だけをもってきても良かったけど資料読みなおしたら
//DXCも必要だったから一緒にクラス化した方が良いと思った
class CompileShaderManager {
private:

	//コンストラクタ
	CompileShaderManager();

	//デストラクタ
	~CompileShaderManager();



public:
	
	//シングルインスタンス
	static CompileShaderManager* GetInstance();

	//コピーコンストラクタ禁止
	CompileShaderManager(const CompileShaderManager& compileShaderManager) = delete;

	//代入演算子を無効にする
	CompileShaderManager& operator=(const CompileShaderManager& compileShaderManager) = delete;

private:

	//DXCの初期化
	void InitializeDXC();


public:
	//CompilerShader関数
	IDxcBlob* CompileShader(
		//CompilerするShaderファイルへのパス
		const std::wstring& filePath,
		//Compilerに使用するProfile
		const wchar_t* profile
		//初期化で生成したものを３つ
		//全部消した
		);


	//解放
	void Release();




private:
	//DXC
	IDxcUtils* dxcUtils_ = nullptr;
	IDxcCompiler3* dxcCompiler_ = nullptr;
	IDxcIncludeHandler* includeHandler_ = nullptr;
	
};
