#include "Easing.h"

#include <cmath>
#include <numbers>

T Easing::EaseInSine(const T& value){
    return static_cast<T>(1.0f - std::cosf((value * std::numbers::pi_v<float>) / 2.0f));
}
