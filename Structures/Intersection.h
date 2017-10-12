#pragma once
#include "../Math/Vec3.h"
#include "Ray.h"
#include "../Object/Object.h"

struct Intersection
{
	bool isHit = false;
	float distance = 0.f;
	Ray inray;
	Vec3 hitPoint;
	Vec3 hitNormal;
	Object* hitObject = nullptr;
	bool isInside = false;
};
