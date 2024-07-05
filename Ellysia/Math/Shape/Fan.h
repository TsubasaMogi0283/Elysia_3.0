#pragma once
#include <Vector3.h>
#include "Vector2.h"



//扇型
struct Fan2D {
	//中心座標
	Vector2 position;
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

//ほぼほぼ円錐かも
struct Fan3D {
	//中心座標
	Vector3 position;
	//長さ
	float length;

	//片側幅の角度
	float sideThetaAngle;
	float sidePhiAngleSize;

	//2等分するベクトル
	Vector3 direction;
	Vector3 rightVector;
	Vector3 leftVector;


	//中心の角度
	float centerRadian;
	float centerPhi;


};