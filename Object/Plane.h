#pragma once
#include "Object.h"

struct Plane : public Object
{
	Vec3 normal;

	Plane();
	Plane(const Vec3& position, const Vec3& normal, const Material& material);
	~Plane();

	void Intersect(const Ray& ray, const Options& options, Intersection& intersection) override;
};

