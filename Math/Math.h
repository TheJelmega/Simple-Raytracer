#pragma once
#include <cmath>

namespace Math {
	
	constexpr float Epsilon = 0.0001f;
	constexpr float Pi = 3.1415926535f;
	constexpr float Deg2Rad = Pi / 180.f;

	inline float ToRadians(float degrees)
	{
		return degrees * Deg2Rad;
	}

	inline float Clamp(float val, float min, float max)
	{
		if (val < min)
			return min;
		if (val > max)
			return max;
		return val;
	}

	inline float Lerp(float lerp, float a, float b)
	{
		return a + lerp * (b - a);
	}

	inline float Min(float a, float b)
	{
		return a < b ? a : b;
	}

	inline float Max(float a, float b)
	{
		return a > b ? a : b;
	}

	inline float Abs(float val)
	{
		return val < 0 ? -val : val;
	}

	inline void Swap(float& a, float& b)
	{
		float c = a;
		a = b;
		b = c;
	}
	
}