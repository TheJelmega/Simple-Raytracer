#pragma once
#include <string>
#include <vector>
#include "../Includes.h"

class Image
{
public:
	Image(size_t width, size_t height);
	~Image();

	void SetPixel(size_t x, size_t y, const Color& color);
	void Save(const std::string& filePath);

private:
	size_t m_Width = 0;
	size_t m_Height = 0;
	std::vector<Color> m_Pixels;
};

