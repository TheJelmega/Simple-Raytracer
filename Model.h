#pragma once
#include "Object/Object.h"
#include "Utilities/Mesh.h"
#include "Structures/AABB.h"
#include "Object/Triangle.h"
#include "Structures/Octree.h"

struct Model : public Object
{
public:
	Model();
	Model(const Vec3& position, float scale, const Material& material, size_t octreeDepth);
	~Model();

	void Init(Mesh* pMesh);

	void Intersect(const Ray& ray, const Options& options, Intersection& intersection) override;

	float scale;
	std::vector<Triangle> tris;
	AABB aabb;
	Octree* octree;
};

