#pragma once
#include "../Math/Vec3.h"
#include "Ray.h"
#include "Options.h"

struct AABB
{
	Vec3 min, max;

	bool Intersect(const Ray& ray, const Options& options)
	{
		float tmin = (min.x - ray.orig.x) / ray.dir.x;
		float tmax = (max.x - ray.orig.x) / ray.dir.x;

		if (tmin > tmax)
			Math::Swap(tmin, tmax);

		float tymin = (min.y - ray.orig.y) / ray.dir.y;
		float tymax = (max.y - ray.orig.y) / ray.dir.y;

		if (tymin > tymax)
			Math::Swap(tymin, tymax);

		if (tmin > tymax || tymin > tmax)
			return false;

		if (tymin > tmin)
			tmin = tymin;

		if (tymax < tmax)
			tmax = tymax;

		float tzmin = (min.z - ray.orig.z) / ray.dir.z;
		float tzmax = (max.z - ray.orig.z) / ray.dir.z;

		if (tzmin > tzmax) Math::Swap(tzmin, tzmax);

		if ((tmin > tzmax) || (tzmin > tmax))
			return false;

		if (tzmin > tmin)
			tmin = tzmin;

		if (tzmax < tmax)
			tmax = tzmax;

		return true;
	}
};
