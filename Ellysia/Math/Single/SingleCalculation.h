#pragma once

/**
 * @file SingleCalculation.h
 * @brief 単体の計算
 * @author 茂木翼
 */

#pragma region 前方宣言

/// <summary>
/// ベクトル(2D)
/// </summary>
struct Vector2;

/// <summary>
/// ベクトル(3D)
/// </summary>
struct Vector3;

#pragma endregion

//単体、例えばint,floatなどの値を返す計算する

/// <summary>
/// 単体の計算
/// </summary>
namespace SingleCalculation{

	/// <summary>
	/// コタンジェント
	/// </summary>
	/// <param name="theta"></param>
	/// <returns></returns>
	float Cot(const float& theta);

	/// <summary>
	/// クランプ
	/// </summary>
	/// <param name="min"></param>
	/// <param name="max"></param>
	/// <param name="t"></param>
	/// <returns></returns>
	float Clamp(const float& min, const float& max, const float& t);

	/// <summary>
	/// 線形補間(float)
	/// </summary>
	/// <param name="start"></param>
	/// <param name="end"></param>
	/// <param name="t"></param>
	/// <returns></returns>
	float Lerp(const float& start, const float& end, const float& t);


	/// <summary>
	/// 長さを求める(Vector2)
	/// </summary>
	/// <param name="v"></param>
	/// <returns></returns>
	float Length(const Vector2& v);

	/// <summary>
	/// 長さを求める(Vector3)
	/// </summary>
	/// <param name="v"></param>
	/// <returns></returns>
	float Length(const Vector3& v);

	/// <summary>
	/// 内積(Vector2)
	/// </summary>
	/// <param name="v1"></param>
	/// <param name="v2"></param>
	/// <returns></returns>
	float Dot(const Vector2& v1, const Vector2& v2);


	/// <summary>
	/// 内積(Vector3)
	/// </summary>
	/// <param name="v1"></param>
	/// <param name="v2"></param>
	/// <returns></returns>
	float Dot(const Vector3& v1, const Vector3& v2);

	


};

