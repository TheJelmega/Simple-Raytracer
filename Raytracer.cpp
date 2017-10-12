#include "Raytracer.h"
#include "Defines.h"

Raytracer::Raytracer()
{
}


Raytracer::~Raytracer()
{
}

void Raytracer::Trace(const Options& options, Scene* pScene, Image* pImg)
{
	auto pCamera = pScene->GetCamera();
	Ray camray;

	for (size_t y = 0; y < options.height; ++y)
	{
		for (size_t x = 0; x < options.width; ++x)
		{
			if (options.antiAlias <= 1)
			{
				pCamera->GetCameraRay(x, y, options, camray);
#ifdef OUTPUT_RAY_DIR
				pImg->SetPixel(x, y, Color(camray.dir.x * .5f + .5f, camray.dir.y * .5f + .5f, -camray.dir.z));
				continue;
#endif
				Intersection intersection;
				TraceScene(camray, options, pScene, intersection);
				pImg->SetPixel(x, y, Shade(intersection, options, pScene, 0));
			}
			else
			{
				Color col;
				for (size_t yAA = 0; yAA < options.antiAlias; ++yAA)
				{
					for (size_t xAA = 0; xAA < options.antiAlias; ++xAA)
					{
						pCamera->GetCameraRay(x, y, xAA, yAA, options, camray);
#ifdef OUTPUT_RAY_DIR
						pImg->SetPixel(x, y, Color(camray.dir.x * .5f + .5f, camray.dir.y * .5f + .5f, -camray.dir.z));
						continue;
#endif
						Intersection intersection;
						TraceScene(camray, options, pScene, intersection);
						col += Shade(intersection, options, pScene, 0);
					}
				}
				pImg->SetPixel(x, y, col * options.cAAColorMultiplier);
			}
		}
	}

	

}

void Raytracer::TraceScene(const Ray& ray, const Options& options, Scene* pScene, Intersection& intersection)
{
	const std::vector<Object*>& pObjects = pScene->GetObjects();
	Intersection tempIntersect;
	//for (Object* pObject : pObjects)
	for (size_t i = 0, numElem = pObjects.size(); i < numElem; ++i)
	{
		Object* pObject = pObjects[i];
		if (pObject)
		{
			pObject->Intersect(ray, options, tempIntersect);
			if (tempIntersect.isHit && tempIntersect.distance <= options.maxRayLength)
			{
				if (!intersection.isHit || tempIntersect.distance < intersection.distance)
					intersection = tempIntersect;
			}
		}
	}
}

inline void Raytracer::TraceReflection(const Intersection& intersection, const Options& options, Scene* pScene, Intersection& relfectIntersection)
{
	Ray reflectRay;
	reflectRay.dir = intersection.inray.dir;
	reflectRay.dir.Reflect(intersection.hitNormal);
	reflectRay.orig = intersection.hitPoint + reflectRay.dir * options.reflectRayOffset;
	TraceScene(reflectRay, options, pScene, relfectIntersection);
}

void Raytracer::TraceRefraction(const Intersection& intersection, const Options& options, Scene* pScene, Intersection& refractIntersection)
{
	Ray refractRay;
	refractRay.dir = intersection.inray.dir;
	refractRay.dir.Refract(intersection.hitNormal, options.ior, intersection.hitObject->mat.ior);
	refractRay.orig = intersection.hitPoint + refractRay.dir * options.refractRayOffset;
	TraceScene(refractRay, options, pScene, refractIntersection);
}

Color Raytracer::Shade(const Intersection& intersection, const Options& options, Scene* pScene, size_t depth)
{
	if (!intersection.isHit)
		return options.backgroundColor;

#ifdef OUTPUT_HIT
	return intersection.isHit ? Color::White : Color::Black;
#endif
#ifdef OUTPUT_DEPTH
	return intersection.isHit ? Color(intersection.distance / options.maxRayLength) : Color::White;
#endif
#ifdef OUTPUT_MAT_COLOR
	return intersection.isHit ? intersection.hitObject->mat.color : options.backgroundColor;
#endif
#ifdef OUTPUT_MAT_COLOR_DEPTH
	return intersection.isHit ? intersection.hitObject->mat.color * Color(intersection.distance / options.maxRayLength) : options.backgroundColor;
#endif

	Color res;

	const std::vector<Light*>& pLights = pScene->GetLights();
	for (Light* pLight : pLights)
	{
		Intersection lightIntersection;
		Ray lightRay;// (pLight->pos, (intersection.hitPoint - pLight->pos).Normalize());
		lightRay.orig = pLight->pos;
		lightRay.dir = intersection.hitPoint - pLight->pos;
		float distanceToLight = lightRay.dir.Length();
		lightRay.dir /= distanceToLight;

		TraceScene(lightRay, options, pScene, lightIntersection);
		if (!lightIntersection.isHit || lightIntersection.distance + options.shadowBias > distanceToLight)
		{
			res += Lambert(intersection, lightIntersection, pLight);
			res += SpecularPhong(intersection, lightIntersection, pLight);
		}
	}

	//Reflection
	if (depth < options.traceDepth)
	{
		if (intersection.hitObject->mat.reflectiveness > Math::Epsilon)
		{
			res += ShadeReflection(intersection, options, pScene, depth) * intersection.hitObject->mat.reflectiveness;
		}
		if (intersection.hitObject->mat.transparency > Math::Epsilon)
		{
			res += ShadeRefraction(intersection, options, pScene, depth);
		}
	}

#ifdef FAKE_AMBIENT
	auto pAmbient = pScene->GetAmbientLight();
	if (pAmbient)
	{
		res += intersection.hitObject->mat.color * pAmbient->col * pAmbient->intensity;
	}
#endif

	return res.Saturate();
}

Color Raytracer::ShadeReflection(const Intersection& intersection, const Options& options, Scene* pScene, size_t depth)
{
	Intersection reflectIntersect;
	TraceReflection(intersection, options, pScene, reflectIntersect);

	if (intersection.isHit)
	{
		return Shade(reflectIntersect, options, pScene, depth + 1);
	}

	return Color::Black;
}

Color Raytracer::ShadeRefraction(const Intersection& intersection, const Options& options, Scene* pScene, size_t depth)
{
	Intersection refractIntersect;
	TraceRefraction(intersection, options, pScene, refractIntersect);

	if (refractIntersect.isHit)
	{
		FresnellResult fr = Fresnell(refractIntersect, options);
		if (fr.kR < Math::Epsilon)
		{
			return Shade(refractIntersect, options, pScene, depth + 1);
		}
		else
		{
			Color res = fr.kT * Shade(refractIntersect, options, pScene, depth + 1);
			res += fr.kR * ShadeReflection(intersection, options, pScene, depth);
			return res;
		}
	}

	return Color::Black;
}

FresnellResult Raytracer::Fresnell(const Intersection& intersection, const Options& options)
{
	float cosi = Math::Clamp(intersection.inray.dir.Dot(intersection.hitNormal), -1.f, 1.f);
	float etai = options.ior, etat = intersection.hitObject->mat.ior;
	if (cosi > 0)
		Math::Swap(etai, etat);

	float sint = etai / etat * sqrt(Math::Max(0.f, 1.f - cosi * cosi));

	if (sint >= 1.f)
		return FresnellResult();

	float cost = sqrt(Math::Max(0.f, 1.f - sint * sint));
	cosi = Math::Abs(cosi);
	float etaiCosi = etai * cosi;
	float etatCosi = etat * cosi;
	float etaiCost = etai * cost;
	float etatCost = etat * cost;
	float rs = (etatCosi - etaiCost) / (etatCosi + etaiCost);
	float rp = (etaiCosi - etatCost) / (etaiCosi + etatCost);
	float kR = (rs * rs + rp * rp) * .5f;
	FresnellResult res;
	res.kR = kR;
	res.kT = 1.f - kR;
	return res;
}

Color Raytracer::Lambert(const Intersection& intersection, const Intersection& LightIntersection, Light* light)
{
	float dot = -intersection.hitNormal.Dot(LightIntersection.inray.dir);
	dot = Math::Clamp(dot, 0.f, 1.f);
	const Material& mat = intersection.hitObject->mat;
	return mat.color * dot * mat.diffuseCoefficient * light->col * light->intensity;
}

Color Raytracer::SpecularPhong(const Intersection& intersection, const Intersection& LightIntersection, Light* light)
{
	Vec3 reflected = LightIntersection.inray.dir.Reflected(intersection.hitNormal);
	float dot = Math::Clamp(-reflected.Dot(intersection.inray.dir), 0.f, 1.f);
	const Material& mat = intersection.hitObject->mat;
	return (1.f - mat.roughness) * light->col * light->intensity * pow(dot, mat.shininess);
}


