#pragma once
/**
 * @file CompileShaderManager.h
 * @brief シェーダーコンパイルクラス
 * @author 茂木翼
 */

#include <string>
#include <cassert>
#include <dxgidebug.h>
#include <wrl.h> 
using Microsoft::WRL::ComPtr;
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
	CompileShaderManager() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~CompileShaderManager()= default;

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
	/// <param name="filePath">CompilerするShaderファイルへのパス</param>
	/// <param name="profile">Compilerに使用するProfile</param>
	/// <returns></returns>
	IDxcBlob* CompileShader(const std::wstring& filePath,const wchar_t* profile);

private:
	//DXC
	ComPtr<IDxcUtils> dxcUtils_ = nullptr;
	ComPtr<IDxcCompiler3> dxcCompiler_ = nullptr;
	ComPtr<IDxcIncludeHandler> includeHandler_ = nullptr;
	
};
