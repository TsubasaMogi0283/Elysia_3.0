#pragma once

/// <summary>
/// エミッタ
/// </summary>
struct Emitter {
	//エミッタのTransform;
	Transform transform;
	//発生数
	uint32_t count;
	//発生頻度
	float_t frequency;
	//頻度用時刻
	float_t frequencyTime;
};