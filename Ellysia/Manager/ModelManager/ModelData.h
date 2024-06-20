#pragma once
#include <vector>
#include <VertexData.h>
#include <Node.h>
#include <map>
#include "JoinWeightData.h"

struct ModelData {
	std::map<std::string, JointWeightData> skinClusterData;
	std::vector<VertexData> vertices;
	std::vector <uint32_t>indices;
	std::string textureFilePath;
	Node rootNode;
};