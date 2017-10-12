#pragma once
#include "../Math/Vec3.h"
#include "../Structures/Material.h"

struct Options;
struct Intersection;
struct Ray;

struct Object
{
	Vec3 pos;
	Material mat;

	Object()
	{}

	Object(const Vec3& position, const Material& material)
		: pos(position)
		, mat(material)
	{}

	virtual ~Object() = default;

	virtual void Intersect(const Ray& ray, const Options& options, Intersection& intersection)
	{}
};
