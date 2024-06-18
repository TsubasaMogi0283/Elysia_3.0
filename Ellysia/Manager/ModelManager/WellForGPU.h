#pragma once
#include <Matrix4x4.h>

//Well...Paletteの色1つ1つを置く場所
struct WellForGPU {
	//位置用
	Matrix4x4 skeletonSpaceMatrix;
	//法線用
	Matrix4x4 skeletonSpaceIncerseTransposeMatrix;
};