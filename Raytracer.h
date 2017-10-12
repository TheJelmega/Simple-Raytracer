#pragma once
#include "Includes.h"

class Raytracer
{
public:
	Raytracer();
	~Raytracer();

	void Trace(const Options& options, Scene* pScene, Image* pImg);

	void TraceScene(const Ray& ray, const Options& options, Scene* pScene, Intersection& intersection);
	inline void TraceReflection(const Intersection& intersection, const Options& options, Scene* pScene, Intersection& relfectIntersection);
	inline void TraceRefraction(const Intersection& intersection, const Options& options, Scene* pScene, Intersection& refractIntersection);

	Color Shade(const Intersection& intersection, const Options& options, Scene* pScene, size_t depth);
	Color ShadeReflection(const Intersection& intersection, const Options& options, Scene* pScene, size_t depth);
	Color ShadeRefraction(const Intersection& intersection, const Options& options, Scene* pScene, size_t depth);

	FresnellResult Fresnell(const Intersection& intersection, const Options& options);

	inline Color Lambert(const Intersection& intersection, const Intersection& LightIntersection, Light* light);
	inline Color SpecularPhong(const Intersection& intersection, const Intersection& LightIntersection, Light* light);
};

