#include "SingleCalculation.h"
#include <cassert>
#include <numbers>
#include <cmath>

float SingleCalculation::Cot(const float& theta){{
	return (1.0f / std::tan(theta));
}
