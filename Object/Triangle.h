#pragma once
#include "../Math/Vec3.h"
#include "Object.h"


struct Intersection;
struct Options;
struct Ray;

struct Triangle
{
	Vec3 v0, e1, e2;
	Vec3 normal;
	Object* parent = nullptr;

	Triangle();
	Triangle(const Vec3& v0, const Vec3& v1, const Vec3& v2);
	Triangle(const Vec3& v0, const Vec3& v1, const Vec3& v2, Object* parent);
	~Triangle();

	void Intersect(const Ray& ray, const Options& options, Intersection& intersection) const;
};

