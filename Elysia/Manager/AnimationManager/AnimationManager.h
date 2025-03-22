#pragma once
/**
 * @file AnimationManager.h
 * @brief  アニメーション管理のクラス
 * @author 茂木翼
 */

#include <array>
#include "Animation.h"


/// <summary>
/// スケルトン
/// </summary>
struct Skeleton;


/// <summary>
/// アニメーション管理クラス
/// </summary>
class AnimationManager{
private:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	AnimationManager() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~AnimationManager() = default;


public:
	
	/// <summary>
	/// インスタンス
	/// </summary>
	/// <returns></returns>
	static AnimationManager* GetInstance();

	/// <summary>
	/// コピーコンストラクタ禁止
	/// </summary>
	/// <param name="modelManager"></param>
	AnimationManager(const AnimationManager& modelManager) = delete;

	/// <summary>
	/// 代入演算子を無効にする
	/// </summary>
	/// <param name="modelManager"></param>
	/// <returns></returns>
	AnimationManager& operator=(const AnimationManager& modelManager) = delete;


private:
	/// <summary>
	/// 読み込み
	/// </summary>
	/// <param name="directoryPath">フォルダ名</param>
	/// <param name="fileName">ファイル名</param>
	/// <returns></returns>
	static Animation LoadAnimationFile(const std::string& directoryPath, const std::string& fileName);

	/// <summary>
	/// 任意の時刻の値を取得(Vector3版)
	/// </summary>
	/// <param name="keyFrames"></param>
	/// <param name="time"></param>
	/// <returns></returns>
	static Vector3 CalculationValue(const std::vector<KeyFrameVector3>& keyFrames, float time);

	/// <summary>
	///  任意の時刻の値を取得(Quaternion版)
	/// </summary>
	/// <param name="keyFrames"></param>
	/// <param name="time"></param>
	/// <returns></returns>
	static Quaternion CalculationValue(const std::vector<KeyFrameQuaternion>& keyFrames, float time);



public:
	/// <summary>
	/// 実際に使う方の読み込み
	/// </summary>
	/// <param name="directoryPath">フォルダ名</param>
	/// <param name="fileName">ファイル名</param>
	/// <returns></returns>
	static uint32_t LoadFile(const std::string& directoryPath, const std::string& fileName);

	/// <summary>
	/// アニメーションの計算
	/// </summary>
	/// <param name="skeleton"></param>
	/// <param name="animationHandle"></param>
	/// <param name="modelHandle"></param>
	/// <param name="animationTime"></param>
	static void ApplyAnimation(Skeleton& skeleton,uint32_t animationHandle,uint32_t modelHandle, float animationTime);


private:
	/// <summary>
	/// アニメーションに関する情報
	/// </summary>
	struct AnimationInformation {
		//モデルデータ
		Animation animationData;

		//ハンドル
		uint32_t handle;

		//ファイル名など
		std::string directoryPath;
		std::string fileName;
	};


private:
	//読み込みの最大数
	static const uint32_t ANIMATION_MAX_AMOUNT_ = 512;

	//このクラスに入っているアニメーションの数
	uint32_t index_=0;

	//mapにしたい
	std::array<AnimationInformation, ANIMATION_MAX_AMOUNT_> animationInfromtion_;


};

