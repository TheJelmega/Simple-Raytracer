#include "Sphere.h"
#include "../Structures/Ray.h"
#include "../Structures/Intersection.h"
#include "../Structures/Options.h"
#include "../Defines.h"

Sphere::Sphere()
	: Object()
	, radius(0.f)
{
}

Sphere::Sphere(const Vec3& position, float radius, const Material& material)
	: Object(position, material)
	, radius(radius)
{
}

Sphere::~Sphere()
{
}

void Sphere::Intersect(const Ray& ray, const Options& options, Intersection& intersection)
{
	intersection.isHit = false;
	intersection.inray = ray;

#ifdef DISABLE_SPHERES
	return;
#endif

	Vec3 l = pos - ray.orig;
	float tca = ray.dir.Dot(l);

	if (tca < 0)
		return;

	float d = sqrt(l.SqLength() - tca * tca);
	if (d < 0)
		return;

	float thc = sqrt(radius * radius - d * d);
	float t0 = tca - thc;
	float t1 = tca + thc;

	if (t0 < t1 && t0 > Math::Epsilon)
		intersection.distance = t0;
	else if (t1 > Math::Epsilon)
		intersection.distance = t1;
	else
		return;

	intersection.isHit = true;
	intersection.hitObject = this;
	intersection.hitPoint = ray.orig + ray.dir * intersection.distance;
	intersection.hitNormal = (intersection.hitPoint - pos).Normalize();
	intersection.isInside = ray.dir.Dot(intersection.hitNormal) > 0;
}
