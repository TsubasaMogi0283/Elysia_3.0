#pragma once
#include <Vector3.h>
#include "Matrix4x4.h"
#include <Vector2.h>


//namespaceを使って分かりやすくしたい
namespace VectorTest {
	void Text();
}


/// <summary>
/// 加算(Vector2)
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <returns></returns>
Vector2 Add(const Vector2& v1,const Vector2& v2);


/// <summary>
/// 加算
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <returns></returns>
Vector3 Add(const Vector3& v1,const Vector3& v2);

/// <summary>
/// 減算
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <returns></returns>
Vector3 Subtract(const Vector3& v1, const Vector3& v2);

/// <summary>
/// クランプ
/// </summary>
/// <param name="t"></param>
/// <param name="min"></param>
/// <param name="max"></param>
/// <returns></returns>
float Clamp(const float& t, const float& min, const float& max);

/// <summary>
/// 内積(Vector3)
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <returns></returns>
float Dot(const Vector3& v1, const Vector3& v2);

/// <summary>
/// 内積(Vector2)
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <returns></returns>
float Dot(const Vector2& v1, const Vector2& v2);

/// <summary>
/// 長さを求める
/// </summary>
/// <param name="V1"></param>
/// <returns></returns>
float Length(const Vector2& V1);


/// <summary>
/// 長さを求める
/// </summary>
/// <param name="V1"></param>
/// <returns></returns>
float Length(const Vector3& V1);

/// <summary>
/// 正規化(Vector2)
/// </summary>
/// <param name="V1"></param>
/// <returns></returns>
Vector2 Normalize(const Vector2& V1);

/// <summary>
/// 正規化(Vector3)
/// </summary>
/// <param name="V1"></param>
/// <returns></returns>
Vector3 Normalize(const Vector3& V1);



/// <summary>
/// 線形補間(float)
/// </summary>
/// <param name="start"></param>
/// <param name="end"></param>
/// <param name="t"></param>
/// <returns></returns>
float Lerp(const float& start, const float& end, const float& t);

/// <summary>
/// 線形補間(Vector3)
/// </summary>
/// <param name="start"></param>
/// <param name="end"></param>
/// <param name="t"></param>
/// <returns></returns>
Vector3 Lerp(const Vector3& start, const Vector3& end, const float& t);

/// <summary>
/// 球面線形補間
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <param name="t"></param>
/// <returns></returns>
Vector3 Slerp(const Vector3& v1, const Vector3& v2, const float& t);


/// <summary>
/// 外積
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <returns></returns>
Vector3 Cross(const Vector3& v1, const Vector3& v2);

Vector3 Project(const Vector3& v1, const Vector3& v2);

Vector3 TransformCalculation(const Vector3& vector, const Matrix4x4& matrix);