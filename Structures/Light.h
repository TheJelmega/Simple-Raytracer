#pragma once
#include "../Math/Vec3.h"
#include "../Math/Color.h"

struct Light
{
	Vec3 pos;
	Color col = Color::White;
	float intensity = 1.f;
};
