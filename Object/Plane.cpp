#include "Plane.h"
#include "../Structures/Ray.h"
#include "../Structures/Intersection.h"
#include "../Structures/Options.h"
#include "../Defines.h"

Plane::Plane()
{
}

Plane::Plane(const Vec3& position, const Vec3& normal, const Material& material)
	: Object(position, material)
	, normal(normal)
{
}

Plane::~Plane()
{
}

void Plane::Intersect(const Ray& ray, const Options& options, Intersection& intersection)
{
	intersection.isHit = false;
	intersection.inray = ray;

#ifdef DISABLE_PLANES
	return;
#endif

	float denom = -normal.Dot(ray.dir);
	if (denom < Math::Epsilon)
		return;

	Vec3 p0l0 = pos - ray.orig;
	float t = -p0l0.Dot(normal) / denom;

	intersection.isHit = true;
	intersection.distance = t;
	intersection.hitObject = this;
	intersection.hitPoint = ray.orig + ray.dir * t;
	intersection.hitNormal = normal;
}
