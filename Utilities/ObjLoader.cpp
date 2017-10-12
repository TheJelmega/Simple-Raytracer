#include "ObjLoader.h"
#include <fstream>
#include <string>


ObjLoader::ObjLoader()
{
}


ObjLoader::~ObjLoader()
{
}

Mesh* ObjLoader::Load(const std::string& filePath)
{
	Mesh* pMesh = new Mesh();
	std::ifstream fin(filePath);
	if (fin.fail())
		return pMesh;

	while (!fin.eof())
	{
		std::string line;
		std::getline(fin, line);

		if (line[0] == '#')
			continue;

		if (line[0] == 'v')
		{
			line = line.substr(2);
			std::string x, y, z;
			int space = line.find(' ');
			if (space == std::string::npos)
				continue;
			x = line.substr(0, space);
			line = line.substr(space + 1);
			space = line.find(' ');
			if (space == std::string::npos)
				continue;
			y = line.substr(0, space);
			line = line.substr(space + 1);
			space = line.find(' ');
			if (space == std::string::npos)
				z = line;
			else
				z = line.substr(0, space);

			Vec3 vertex(std::stof(x), std::stof(y), std::stof(z));
			pMesh->AddVertex(vertex);
		}

		if (line[0] == 'f')
		{
			line = line.substr(2);
			std::string i1, i2, i3;
			int space = line.find(' ');
			if (space == std::string::npos)
				continue;
			i1 = line.substr(0, space);
			line = line.substr(space + 1);
			space = line.find(' ');
			if (space == std::string::npos)
				continue;
			i2 = line.substr(0, space);
			line = line.substr(space + 1);
			space = line.find(' ');
			if (space == std::string::npos)
				i3 = line;
			else
				i3 = line.substr(0, space);

			pMesh->AddIndex(static_cast<unsigned short>(std::stoi(i1) - 1));
			pMesh->AddIndex(static_cast<unsigned short>(std::stoi(i2) - 1));
			pMesh->AddIndex(static_cast<unsigned short>(std::stoi(i3) - 1));
		}
	}

	fin.close();
	return pMesh;
}
