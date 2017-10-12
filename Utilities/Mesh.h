#pragma once
#include <vector>
#include "../Math/Vec3.h"

class Mesh
{
public:
	Mesh();
	~Mesh();

	void AddVertex(const Vec3& vert) { m_Vertices.push_back(vert); }
	std::vector<Vec3>& GetVertices() { return m_Vertices; }

	void AddIndex(unsigned short index) { m_Indices.push_back(index); }
	std::vector<unsigned short>& GetIndices() { return m_Indices; }

	void Load(const std::string& filepath);

private:
	std::vector<Vec3> m_Vertices;
	std::vector<unsigned short> m_Indices;
};

