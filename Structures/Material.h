#pragma once
#include "../Math/Color.h"

struct Material
{
	Color color;
	float diffuseCoefficient = 1.f;
	float roughness = 1.f;
	float shininess = 0.f;
	float reflectiveness = 0.f;
	float transparency;
	float ior = 1.f;
};
