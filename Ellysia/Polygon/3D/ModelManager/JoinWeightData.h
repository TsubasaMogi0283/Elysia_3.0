#pragma once
#include <Matrix4x4.h>
#include <vector>
#include <assimp/defs.h>

struct JointWeightData {
	Matrix4x4 inverseBindPoseMatrix;
	std::vector<std::pair<ai_real, unsigned int >> vertexWeights;
};