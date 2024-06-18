#pragma once
#include <Matrix4x4.h>
#include <string>
#include <vector>
#include <QuaternionTransform.h>

//親からどんどん子に伸びていく

struct Node {
	QuaternionTransform transform;
	Matrix4x4 localMatrix;
	std::string name;
	std::vector<Node> children;
};