#pragma once
#include <cstdint>


//未定。仮置きみたいなもの
//0
const uint32_t COLLISION_ATTRIBUTE_NONE = 0b0;

//基本2進数で
//1
//プレイヤー陣営
const uint32_t COLLISION_ATTRIBUTE_PLAYER = 0b1;

//敵陣営
//2
//左に1つシフトする
const uint32_t COLLISION_ATTRIBUTE_ENEMY = 0b1 << 1;

//敵の攻撃の当たり判定
//4
//左に2つシフトする
const uint32_t COLLISION_ATTRIBUTE_ENEMY_ATTACK = 0b1 << 2;


//一発アウトの敵の攻撃の当たり判定
//8
//左に3つシフトする
const uint32_t COLLISION_ATTRIBUTE_STRONG_ENEMY = 0b1 << 3;

//プレイヤー(2)
//16
//左に4つシフトする
const uint32_t COLLISION_ATTRIBUTE_PLAYER2 = 0b1 << 4;


