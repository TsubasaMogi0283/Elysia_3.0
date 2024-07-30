#include "VectorCalculation.h"

#include <corecrt_math.h>
#include <cassert>
#include <numbers>
#include <cmath>
#include <SingleCalculation.h>



Vector2 VectorCalculation::Add(const Vector2& v1, const Vector2& v2){
	Vector2 result = {
		.x = v1.x + v2.x,
		.y = v1.x + v2.y
	};

	
	return result;
}

Vector3 VectorCalculation::Add(const Vector3& v1, const Vector3& v2) {
	Vector3 result = {
		.x = v1.x + v2.x,
		.y = v1.y + v2.y,
		.z = v1.z + v2.z
	};
	return result;
}

Vector2 VectorCalculation::Subtract(const Vector2& v1, const Vector2& v2){
	Vector2 result = {
		.x = v1.x - v2.x,
		.y = v1.y - v2.y
	};

	return result;
}

Vector3 VectorCalculation::Subtract(const Vector3& v1, const Vector3& v2){
	Vector3 result = {
		.x = v1.x - v2.x,
		.y = v1.y - v2.y,
		.z = v1.z - v2.z
	};
	
	return result;
}

Vector2 VectorCalculation::Normalize(const Vector2& v){
	Vector2 result = {};

	//長さを求める
	float length = sqrtf(v.x * v.x + v.y * v.y);

	float newX = v.x;
	float newY = v.y;

	//0除算を防ぐ
	if (length != 0.0f) {
		newX = v.x / length;
		newY = v.y / length;
	}

	result.x = newX;
	result.y = newY;


	return result;
}

Vector3 VectorCalculation::Normalize(const Vector3& v){
	Vector3 result = {};
	
	//長さを求める
	float length = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);

	float newX = v.x;
	float newY = v.y;
	float newZ = v.z;

	//0除算を防ぐ
	if (length != 0.0f) {
		newX = v.x / length;
		newY = v.y / length;
		newZ = v.z / length;

	}

	result.x = newX;
	result.y = newY;
	result.z = newZ;


	return result;
}

Vector3 VectorCalculation::Lerp(const Vector3& v1, const Vector3& v2, const float& t){
	Vector3 result = {};

	result.x = (1.0f - t) * v1.x + t * v2.x;
	result.y = (1.0f - t) * v1.y + t * v2.y;
	result.z = (1.0f - t) * v1.z + t * v2.z;

	return result;
}

Vector3 VectorCalculation::Slerp(const Vector3& v1, const Vector3& v2, const float& t){
	float newT = SingleCalculation::Clamp(t, 0.0f, 1.0f);

	//正規化
	Vector3 normalizeV1 = Normalize(v1);
	Vector3 normalizeV2 = Normalize(v2);

	//内積
	float dot = SingleCalculation::Dot(normalizeV1, normalizeV2);

	//θを求める
	float theta = std::acosf(dot) * newT;

	//差分
	Vector3 subtractVector = Subtract(v2, v1);
	
	//差分にnewTをかける
	Vector3 newSubtractVector = {
		subtractVector.x * newT,
		subtractVector.y * newT,
		subtractVector.z * newT };
	
	//正規化
	Vector3 relativeVector = Normalize(newSubtractVector);

	Vector3 result = {
		v1.x * std::cos(theta) + relativeVector.x * std::sin(theta),
		v1.y * std::cos(theta) + relativeVector.y * std::sin(theta),
		v1.z * std::cos(theta) + relativeVector.z * std::sin(theta)
	};

	return result;
}

Vector3 VectorCalculation::Cross(const Vector3& v1, const Vector3& v2){
	Vector3 result = {
		.x = v1.y * v2.z - v1.z * v2.y,
		.y = v1.z * v2.x - v1.x * v2.z,
		.z = v1.x * v2.y - v1.y * v2.x
	};

	return result;
}

Vector3 VectorCalculation::Project(const Vector3& v1, const Vector3& v2) {

	//内積
	float dot = SingleCalculation::Dot(v1, v2);
	//長さ
	float length = SingleCalculation::Length(v2);


	float t = dot / (length * length);

	Vector3 result = {};
	result.x = t * v2.x;
	result.y = t * v2.y;
	result.z = t * v2.z;


	return result;

}


Vector3 VectorCalculation::TransformCalculation(const Vector3& v, const Matrix4x4& m) {
	Vector3 result = {
		.x = (v.x * m.m[0][0]) + (v.y * m.m[1][0]) + (v.z * m.m[2][0]) + (1.0f * m.m[3][0]),
		.y = (v.x * m.m[0][1]) + (v.y * m.m[1][1]) + (v.z * m.m[2][1]) + (1.0f * m.m[3][1]),
		.z = (v.x * m.m[0][2]) + (v.y * m.m[1][2]) + (v.z * m.m[2][2]) + (1.0f * m.m[3][2])
	};
	float w = (v.x * m.m[0][3]) + (v.y * m.m[1][3]) + (v.z * m.m[2][3]) + (1.0f * m.m[3][3]);

	//0除算を避ける
	if (w != 0.0f) {
		result.x /= w;
		result.y /= w;
		result.z /= w;

	}
	
	return result;


}

