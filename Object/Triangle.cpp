#include "Triangle.h"
#include "../Structures/Ray.h"
#include "../Structures/Intersection.h"
#include "../Structures/Options.h"
#include "../Defines.h"

Triangle::Triangle()
{
}

Triangle::Triangle(const Vec3& v0, const Vec3& v1, const Vec3& v2)
	: v0(v0)
{
	e1 = v1 - v0;
	e2 = v2 - v0;
	normal = e1.Cross(e2).Normalize();
}

Triangle::Triangle(const Vec3& v0, const Vec3& v1, const Vec3& v2, Object* parent)
	: v0(v0)
	, parent(parent)
{
	e1 = v1 - v0;
	e2 = v2 - v0;
	normal = e1.Cross(e2).Normalize();
}

Triangle::~Triangle()
{
}

void Triangle::Intersect(const Ray& ray, const Options& options, Intersection& intersection) const
{
	intersection.isHit = false;
	intersection.inray = ray;

#ifdef DISABLE_TRIANGLES
	//return;
#endif

	Vec3 pvec = ray.dir.Cross(e2);
	float det = e1.Dot(pvec);

	if (options.cullTriangle)
	{
		if (det < Math::Epsilon)
			return;
	}
	else if (Math::Abs(det) < Math::Epsilon)
		return;

	float invDet = 1.f / det;

	Vec3 tvec = ray.orig - v0;
	float u = tvec.Dot(pvec) * invDet;
	if (u < 0.f || u > 1.f)
		return;

	Vec3 qvec = tvec.Cross(e1);
	float v = ray.dir.Dot(qvec) * invDet;
	if (v < 0.f || u + v > 1.f)
		return;

	float t = e2.Dot(qvec) * invDet;

	if (t < Math::Epsilon)
		return;

	intersection.isHit = true;
	intersection.distance = t;
	intersection.hitObject = parent;
	intersection.hitPoint = ray.orig + ray.dir * t;
	intersection.hitNormal = normal;
}
