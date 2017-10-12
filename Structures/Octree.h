#pragma once
#include "../Math/Vec3.h"
#include "OctreeNode.h"
#include "Intersection.h"

struct Octree
{
	OctreeNode* mainNode;


	Octree(size_t depth)
	{
		mainNode = new OctreeNode(0, depth);
	}

	~Octree()
	{
		delete mainNode;
	}

	void SetSizeAndPosition(const Vec3& position, const Vec3& halfExtends)
	{
		mainNode->SetSizeAndPos(position, halfExtends);
	}

	void AddTriangle(const Triangle& triangle)
	{
		mainNode->AddTriangle(triangle);
	}

	void TraceOctree(const Ray& ray, const Options& options, Intersection& intersection)
	{
		float curDist = 0.f;
		OctreeNode* curNode = nullptr;

		while (true)
		{
			float tempDist = 0.f;
			OctreeNode* tempNode = mainNode->GetCurrentClosestNode(ray, options, curNode, curDist, tempDist);

			if (tempNode == curNode || tempNode == nullptr)
				break;

			curNode = tempNode;
			curDist = tempDist;

			Intersection triIntersect;
			for (const Triangle& tri : curNode->containedTris)
			{
				tri.Intersect(ray, options, triIntersect);
				if (triIntersect.isHit && triIntersect.distance < options.maxRayLength)
				{
					if (intersection.isHit == false || triIntersect.distance < intersection.distance)
						intersection = triIntersect;
				}
			}

			if (intersection.isHit)
				break;
		}
	}
};
