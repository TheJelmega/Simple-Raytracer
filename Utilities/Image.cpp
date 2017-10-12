#include "Image.h"
#include "../lodepng/lodepng.h"
#include <iostream>


Image::Image(size_t width, size_t height)
	: m_Width(width)
	, m_Height(height)
{
	m_Pixels.assign(m_Width * m_Height, Color());
}


Image::~Image()
{
}

void Image::SetPixel(size_t x, size_t y, const Color& color)
{
	if (x > m_Width || y > m_Height)
		return;
	int i = y * m_Width + x;
	m_Pixels[i] = color;
}

void Image::Save(const std::string& filePath)
{
	std::vector<unsigned char> pixels;
	pixels.reserve(m_Width * m_Height);
	for (Color color : m_Pixels)
	{
		color.ToRGB255();
		pixels.push_back(static_cast<unsigned char>(color.r));
		pixels.push_back(static_cast<unsigned char>(color.g));
		pixels.push_back(static_cast<unsigned char>(color.b));
		pixels.push_back(static_cast<unsigned char>(color.a));
	}

	int error = lodepng::encode(filePath, pixels, m_Width, m_Height);
	if (error != 0)
	{
		std::cerr << "Lodepng encountered a problem saving " + filePath + ", Error: " + lodepng_error_text(error) + "!\n";
	}
}
