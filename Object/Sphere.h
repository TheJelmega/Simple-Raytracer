#pragma once
#include "Object.h"


struct Sphere : public Object
{
	float radius;

	Sphere();
	Sphere(const Vec3& position, float radius, const Material& material);
	~Sphere();


	void Intersect(const Ray& ray, const Options& options, Intersection& intersection) override;
};

