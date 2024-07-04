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

	//片側の角度
	float sideAngle;

	//2等分するベクトル
	Vector2 direction;
	Vector2 rightVector;
	Vector2 leftVector;


	//左側の角度
	float leftSideRadian;
	//中心の角度
	float centerRadian;
	//右側の角度
	float rightSideRadian;


};


