#pragma once
#include <Vector3.h>

//扇型
struct Fan {
	//中心座標
	Vector3 position;
	//範囲(角度)
	float range;
	//長さ
	float length;
	//方向(角度)
	float directionDegree;
};


