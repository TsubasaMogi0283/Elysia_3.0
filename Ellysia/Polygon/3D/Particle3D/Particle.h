#pragma once
#include "Transform.h"
#include "Vector4.h"
#include "Matrix4x4.h"

/// <summary>
/// パーティクルの情報
/// </summary>
struct Particle {
	Transform transform;
	Vector3  velocity;
	Vector4 color;
	float lifeTime;
	float currentTime;
	bool isInvisible;
};

/// <summary>
/// GPUに送る方のパーティクル
/// </summary>
struct ParticleForGPU {
	Matrix4x4  world;
	Vector4 color;
};
