#pragma once
#include <Vector3.h>
#include "Matrix4x4.h"
#include <Vector2.h>

namespace VectorCalculation {

	/// <summary>
	/// 加算(Vector2)
	/// </summary>
	/// <param name="v1">ベクトル1</param>
	/// <param name="v2">ベクトル2</param>
	/// <returns></returns>
	Vector2 Add(const Vector2& v1, const Vector2& v2);

	/// <summary>
	/// 加算(Vector3)
	/// </summary>
	/// <param name="v1">ベクトル1</param>
	/// <param name="v2">ベクトル2</param>
	/// <returns></returns>
	Vector3 Add(const Vector3& v1, const Vector3& v2);

	/// <summary>
	/// 減算(Vector2)
	/// </summary>
	/// <param name="v1">ベクトル1</param>
	/// <param name="v2">ベクトル2</param>
	/// <returns></returns>
	Vector2 Subtract(const Vector2& v1, const Vector2& v2);

	/// <summary>
	/// 減算(Vector3)
	/// </summary>
	/// <param name="v1">ベクトル1</param>
	/// <param name="v2">ベクトル2</param>
	/// <returns></returns>
	Vector3 Subtract(const Vector3& v1, const Vector3& v2);

	/// <summary>
	/// 正規化(Vector2)
	/// </summary>
	/// <param name="v">ベクトル</param>
	/// <returns></returns>
	Vector2 Normalize(const Vector2& v);

	/// <summary>
	/// 正規化(Vector3)
	/// </summary>
	/// <param name="v">ベクトル</param>
	/// <returns></returns>
	Vector3 Normalize(const Vector3& v);


	/// <summary>
	/// 線形補間(Vector3)
	/// </summary>
	/// <param name="v1">ベクトル1</param>
	/// <param name="v2">ベクトル2</param>
	/// <param name="t">t</param>
	/// <returns></returns>
	Vector3 Lerp(const Vector3& v1, const Vector3& v2, const float& t);

	/// <summary>
	/// 球面線形補間
	/// </summary>
	/// <param name="v1">ベクトル1</param>
	/// <param name="v2">ベクトル2</param>
	/// <param name="t">t</param>
	/// <returns></returns>
	Vector3 Slerp(const Vector3& v1, const Vector3& v2, const float& t);

	/// <summary>
	/// 外積
	/// </summary>
	/// <param name="v1">ベクトル1</param>
	/// <param name="v2">ベクトル2</param>
	/// <returns></returns>
	Vector3 Cross(const Vector3& v1, const Vector3& v2);


	/// <summary>
	/// 正射影ベクトル
	/// </summary>
	/// <param name="v1">ベクトル1</param>
	/// <param name="v2">ベクトル2</param>
	/// <returns></returns>
	Vector3 Project(const Vector3& v1, const Vector3& v2);


	
	/// <summary>
	/// 回転
	/// </summary>
	/// <param name="v">ベクトル</param>
	/// <param name="m">行列</param>
	/// <returns></returns>
	Vector3 TransformCalculation(const Vector3& v, const Matrix4x4& m);


}