#pragma once
/**
 * @file CompileShaderManager.h
 * @brief シェーダーコンパイルクラス
 * @author 茂木翼
 */

#include <string>
#include <cassert>

#include <dxgidebug.h>
#include "ConvertLog.h"




#include <dxcapi.h>
#pragma comment(lib,"dxcompiler.lib")



/// <summary>
/// シェーダーコンパイルクラス
/// </summary>
class CompileShaderManager {
private:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CompileShaderManager();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~CompileShaderManager();



public:
	
	/// <summary>
	/// インスタンスの取得
	/// </summary>
	/// <returns></returns>
	static CompileShaderManager* GetInstance();

	/// <summary>
	/// コピーコンストラクタ禁止
	/// </summary>
	/// <param name="compileShaderManager"></param>
	CompileShaderManager(const CompileShaderManager& compileShaderManager) = delete;

	/// <summary>
	/// 代入演算子を無効にする
	/// </summary>
	/// <param name="compileShaderManager"></param>
	/// <returns></returns>
	CompileShaderManager& operator=(const CompileShaderManager& compileShaderManager) = delete;

private:

	/// <summary>
	/// DXCの初期化
	/// </summary>
	void InitializeDXC();


public:
	/// <summary>
	/// CompilerShader関数
	/// </summary>
	/// <param name="filePath"></param>
	/// <param name="profile"></param>
	/// <returns></returns>
	IDxcBlob* CompileShader(
		//CompilerするShaderファイルへのパス
		const std::wstring& filePath,
		//Compilerに使用するProfile
		const wchar_t* profile
		//初期化で生成したものを３つ
		//全部消した
		);


	/// <summary>
	/// 解放
	/// </summary>
	void Release();




private:
	//DXC
	IDxcUtils* dxcUtils_ = nullptr;
	IDxcCompiler3* dxcCompiler_ = nullptr;
	IDxcIncludeHandler* includeHandler_ = nullptr;
	
};
