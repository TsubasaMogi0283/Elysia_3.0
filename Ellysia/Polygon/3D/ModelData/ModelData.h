#pragma once
#include <vector>
#include <VertexData.h>
#include "MaterialData.h"
#include <Node.h>


struct ModelData {
	std::vector<VertexData> vertices;
	MaterialData material;
	Node rootNode;
};