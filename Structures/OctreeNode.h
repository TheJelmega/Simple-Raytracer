#pragma once
#include "../Math/Vec3.h"
#include "Ray.h"
#include "Options.h"
#include <vector>
#include "../Object/Triangle.h"


struct OctreeNode
{
	Vec3 pos, halfExtends;

	size_t depth, maxDepth;
	OctreeNode* nodes[8];
	bool isLeaf = false;
	std::vector<Triangle> containedTris;

	OctreeNode(size_t depth, size_t maxDepth)
		: depth(depth)
		, maxDepth(maxDepth)
	{
		if (depth < maxDepth)
		{
			for (int i = 0; i < 8; ++i)
			{
				nodes[i] = new OctreeNode(depth + 1, maxDepth);
			}
		}
		else
			isLeaf = true;

	}

	~OctreeNode()
	{
		if (!isLeaf)
		{
			for (OctreeNode*& node : nodes)
			{
				delete node;
				node = nullptr;
			}
		}
	}

	void SetSizeAndPos(const Vec3& position, const Vec3& halfExtends)
	{
		this->halfExtends = halfExtends;
		this->pos = position;
		if (isLeaf)
			return;
		Vec3 nodeHalfExtends = halfExtends * .5f;
		for (int i = 0; i < 8; ++i)
		{
			Vec3 nodePosition = position;
			nodePosition.x += i & 1 ? -nodeHalfExtends.x : nodeHalfExtends.x;
			nodePosition.y += i & 2 ? -nodeHalfExtends.y : nodeHalfExtends.y;
			nodePosition.z += i & 4 ? -nodeHalfExtends.z : nodeHalfExtends.z;
			nodes[i]->SetSizeAndPos(nodePosition, nodeHalfExtends);
		}
	}

	void AddTriangle(const Triangle& tri)
	{
		if (isLeaf)
			containedTris.push_back(tri);
		else
		{
			for (int i = 0; i < 8; ++i)
			{
				OctreeNode* node = nodes[i];
				Vec3 min = node->pos - node->halfExtends;
				Vec3 max = node->pos + node->halfExtends;
				if (tri.v0.isInside(min, max))
					node->AddTriangle(tri);
				else if ((tri.v0 + tri.e1).isInside(min, max))
					node->AddTriangle(tri);
				else if ((tri.v0 + tri.e2).isInside(min, max))
					node->AddTriangle(tri);
			}
		}

	}

	OctreeNode* GetCurrentClosestNode(const Ray& ray, const Options& options, OctreeNode* previousNode, float previousSqDistance, float& curNodeSqDistance)
	{
		if (isLeaf)
		{
			if (this == previousNode)
				return previousNode;
			
			if (!Intersect(ray, options))
				return previousNode;

			float sqDist = (pos - ray.orig).SqLength();
			if (sqDist > previousSqDistance)
			{
				curNodeSqDistance = sqDist;
				return this;
			}
			return previousNode;
		}
		else
		{
			if (!Intersect(ray, options))
				return previousNode;

			float tempDist = std::numeric_limits<float>::max();
			OctreeNode* tempNode = nullptr;

			for (auto node : nodes)
			{
				float nodeDist;
				auto tNode = node->GetCurrentClosestNode(ray, options, previousNode, previousSqDistance, nodeDist);
				if (tNode)
				{
					if (nodeDist > previousSqDistance && nodeDist < tempDist && tNode != previousNode)
					{
						tempDist = nodeDist;
						tempNode = tNode;
					}
				}
			}

			if (tempNode == nullptr)
				return previousNode;

			curNodeSqDistance = tempDist;
			return tempNode;
		}
	}

	bool Intersect(const Ray& ray, const Options& options)
	{
		Vec3 max = pos + halfExtends;
		Vec3 min = pos - halfExtends;

		float tmin = (min.x - ray.orig.x) / ray.dir.x;
		float tmax = (max.x - ray.orig.x) / ray.dir.x;

		if (tmin > tmax)
			Math::Swap(tmin, tmax);

		float tymin = (min.y - ray.orig.y) / ray.dir.y;
		float tymax = (max.y - ray.orig.y) / ray.dir.y;

		if (tymin > tymax)
			Math::Swap(tymin, tymax);

		if (tmin > tymax || tymin > tmax)
			return false;

		if (tymin > tmin)
			tmin = tymin;

		if (tymax < tmax)
			tmax = tymax;

		float tzmin = (min.z - ray.orig.z) / ray.dir.z;
		float tzmax = (max.z - ray.orig.z) / ray.dir.z;

		if (tzmin > tzmax) Math::Swap(tzmin, tzmax);

		if ((tmin > tzmax) || (tzmin > tmax))
			return false;

		if (tzmin > tmin)
			tmin = tzmin;

		if (tzmax < tmax)
			tmax = tzmax;

		return true;
	}
};
