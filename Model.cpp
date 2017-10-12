#include "Model.h"
#include "Math/Mat4.h"
#include "Structures/Intersection.h"
#include "Structures/Options.h"


Model::Model()
	: Object()
{
}

Model::Model(const Vec3& position, float scale, const Material& material, size_t octreeDepth)
	: Object(position, material)
	, scale(scale)
{
	octree = new Octree(octreeDepth);
}


Model::~Model()
{
	delete octree;
}

void Model::Init(Mesh* pMesh)
{
	const std::vector<Vec3>& verts = pMesh->GetVertices();
	const std::vector<unsigned short>& indices = pMesh->GetIndices();
	size_t numVerts = verts.size();
	size_t numIndices = indices.size();
	size_t numTris = numIndices / 3;

	// Generate AABB
	for (size_t i = 0; i < numVerts; ++i)
	{
		if (i == 0)
		{
			aabb.min = aabb.max = verts[i];
		}
		else
		{
			Vec3 vert = verts[i];
			aabb.min = Vec3(Math::Min(aabb.min.x, vert.x), Math::Min(aabb.min.y, vert.y), Math::Min(aabb.min.z, vert.z));
			aabb.max = Vec3(Math::Max(aabb.max.x, vert.x), Math::Max(aabb.max.y, vert.y), Math::Max(aabb.max.z, vert.z));
		}
	}

	Mat4 scaleTranslation;
	scaleTranslation.ToScaleTranslation(pos, scale);
	Vec4 temp(aabb.min.x, aabb.min.y, aabb.min.z);
	temp = temp * scaleTranslation;
	aabb.min = Vec3(temp.x, temp.y, temp.z);
	temp = Vec4(aabb.max.x, aabb.max.y, aabb.max.z);
	temp = temp * scaleTranslation;
	aabb.max = Vec3(temp.x, temp.y, temp.z);

	octree->SetSizeAndPosition((aabb.min + aabb.max) * .5f, (aabb.max - aabb.min) * .5f);

	//Scale and Translate
	std::vector<Vec3> finalVerts;
	finalVerts.reserve(numVerts);
	for (size_t i = 0; i < numVerts; ++i)
	{
		const Vec3& vert = verts[i];
		temp = Vec4(vert.x, vert.y, vert.z);
		temp = temp * scaleTranslation;
		finalVerts.push_back(Vec3(temp.x, temp.y, temp.z));
	}

	//Generate tris
	size_t numTriIndices = numTris * 3;
	for (size_t i = 0; i < numTriIndices;)
	{
		Vec3 v0 = finalVerts[indices[i++]];
		Vec3 v1 = finalVerts[indices[i++]];
		Vec3 v2 = finalVerts[indices[i++]];

		//tris.push_back(Triangle(v0, v1, v2, this));
		octree->AddTriangle(Triangle(v0, v1, v2, this));
	}
}

void Model::Intersect(const Ray& ray, const Options& options, Intersection& intersection)
{
	if (aabb.Intersect(ray, options))
	{
		/*Intersection tempIntersect;
		for (const Triangle& tri : tris)
		{
			tri.Intersect(ray, options, tempIntersect);
			if (tempIntersect.isHit && tempIntersect.distance <= options.maxRayLength)
			{
				if (!intersection.isHit || tempIntersect.distance < intersection.distance)
				{
					intersection = tempIntersect;
				}
			}
		}*/

		octree->TraceOctree(ray, options, intersection);
	}
	else
		intersection.inray = ray;
}
