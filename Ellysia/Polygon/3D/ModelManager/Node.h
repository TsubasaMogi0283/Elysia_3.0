#pragma once
#include <Matrix4x4.h>
#include <string>
#include <vector>

//親からどんどん子に伸びていく

struct Node {
	Matrix4x4 localMatrix;
	std::string name;
	std::vector<Node> children;
};