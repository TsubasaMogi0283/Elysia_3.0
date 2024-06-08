#pragma once
#include <cstdint>
#include <array>

//VSでの
//float32_t4 weight:WEIGHT0;
//int32_t index:INDEX0
//これと形を同じにしてね

const uint32_t NUM_MAX_INFLUENCE = 4;
struct VertexInfluence {
	std::array<float, NUM_MAX_INFLUENCE> weights;
	std::array<int32_t, NUM_MAX_INFLUENCE> jointIndices;
};