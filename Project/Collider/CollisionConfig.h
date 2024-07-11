#pragma once
#include <cstdint>


//基本2進数で
//1
//プレイヤー陣営
const uint32_t COLLISION_ATTRIBUTE_PLAYER = 0b1;


//敵陣営
//2
//左に1つシフトする
const uint32_t COLLISION_ATTRIBUTE_ENEMY = 0b1 << 1;

//オブジェクト
//4
//左に2つシフトする
const uint32_t COLLISION_ATTRIBUTE_OBJECT = 0b1 << 2;

//プレイヤー陣営の武器
//8
//左に2つシフトする
const uint32_t COLLISION_ATTRIBUTE_P_WEAPON = 0b1 << 3;

//敵陣営の武器
//16
//左に3つシフトする
const uint32_t COLLISION_ATTRIBUTE_E_WEAPON = 0b1 << 4;

