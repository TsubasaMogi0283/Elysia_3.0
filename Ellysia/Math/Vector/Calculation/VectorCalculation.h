#pragma once
#include <Vector3.h>
#include "Matrix4x4.h"

//Vector3

//足し算
Vector3 Add(Vector3 v1, Vector3 v2);

//引き算
Vector3 Subtract(Vector3 v1, Vector3 v2);

float Clamp(float t, float min, float max);

float DotVector3(const Vector3 v1, const Vector3 v2);
float DotVector2(const Vector3 v1, const Vector3 v2);

float Length(Vector3 V1);
Vector3 Normalize(Vector3 V1);

float Cot(float theta);

Vector3 Cross(const Vector3 v1, const Vector3 v2);


Vector3 Project(const Vector3 v1, const Vector3 v2);

Vector3 TransformCalculation(const Vector3 vector, const Matrix4x4 matrix);