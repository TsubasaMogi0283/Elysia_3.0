#pragma once
#include "Vector4.h"
#include <cstdint>
#include "Matrix4x4.h"

struct Material {
	Vector4 color;
	//boolの代わりにint32_t
	int32_t enableLighting;
	float padding[3];
	Matrix4x4 uvTransform;
};