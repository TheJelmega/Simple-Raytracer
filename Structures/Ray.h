#pragma once
#include "../Math/Vec3.h"
#include <algorithm>

struct Ray
{
	Vec3 orig;
	Vec3 dir;

	Ray()
	{}

	Ray(const Vec3& origin, const Vec3& direction)
		: orig(origin)
		, dir(direction)
	{}

	Ray(const Ray& r)
		: orig(r.orig)
		, dir(r.dir)
	{}

	Ray(Ray&& r) noexcept
	{
		std::swap(orig, r.orig);
		std::swap(dir, r.dir);
	}

	Ray& operator=(const Ray& r)
	{
		orig = r.orig;
		dir = r.dir;
		return *this;
	}

	Ray& operator=(Ray&& r) noexcept
	{
		std::swap(orig, r.orig);
		std::swap(dir, r.dir);
		return *this;
	}
};
