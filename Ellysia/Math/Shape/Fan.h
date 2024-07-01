#pragma once
#include <Vector3.h>
#include "Vector2.h"
//扇型
struct Fan {
	//中心座標
	Vector2 position;
	//範囲(角度)
	float range;
	//長さ
	float length;
	//方向(角度)
	float directionRadian;
	//2等分するベクトル
	Vector2 devidDirection;
};


