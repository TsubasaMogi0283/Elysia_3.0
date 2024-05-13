#pragma once
#include <vector>
#include <VertexData.h>
#include "MaterialData.h"
#include <Node.h>


struct ModelData {
	std::vector<VertexData> vertices;
	std::vector <uint32_t>indices;
	MaterialData material;
	Node rootNode;
};