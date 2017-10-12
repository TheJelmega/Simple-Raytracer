#pragma once
#include "../Math/Vec3.h"
#include "../Math/Color.h"
#include "../Object/Object.h"
#include <iostream>

struct Options
{
	size_t width = 0;
	size_t height = 0;
	float fov = 0.f;

	size_t traceDepth = 0;
	bool cullTriangle = false;

	float maxRayLength = 1000.f;

	Color backgroundColor = Color::Black;

	size_t antiAlias = 1;

	float ior = 1.f;

	float shadowBias = Math::Epsilon;
	float reflectRayOffset = Math::Epsilon;
	float refractRayOffset = Math::Epsilon;

	float cInvHalfWidth = 0.f;
	float cInvHalfHeight = 0.f;
	float cTanHalfFov = 0.f;
	float cAspect = 0.f;
	float cAAOffset = 0.f;
	float cAAColorMultiplier = 1.f;

	void Calculate()
	{
		cInvHalfWidth = 2.f / width;
		cInvHalfHeight = 2.f / width;
		cAspect = float(width) / float(height);
		cTanHalfFov = tan(fov * .5f);
		cAAOffset = 1.f / float(antiAlias + 1);
		cAAColorMultiplier = 1.f / float(antiAlias * antiAlias);
	}
};
