#pragma once
#include "Math.h"

struct Color
{
	float r, g, b, a;

	Color()
		: r(0.f), g(0.f), b(0.f), a(1.f)
	{}

	Color(float greyscale)
		: r(greyscale), g(greyscale), b(greyscale), a(1.f)
	{}

	Color(float r, float g, float b)
		: r(r), g(g), b(b), a(1.f)
	{}

	Color(float r, float g, float b, float a)
		: r(r), g(g), b(b), a(a)
	{}

	Color(const Color& c)
		: r(c.r), g(c.g), b(c.b), a(c.a)
	{
		
	}

	Color(Color&& c) noexcept
		: r(c.r), g(c.g), b(c.b), a(c.a)
	{
	}

	Color operator+(const Color& c) const
	{
		return Color(r + c.r, g + c.g, b + c.b, a + c.a);
	}

	Color operator*(const Color& c) const
	{
		return Color(r * c.r, g * c.g, b * c.b, a * c.a);
	}

	Color operator*(float val) const
	{
		return Color(r * val, g * val, b * val, a * val);
	}

	Color& operator=(const Color& c)
	{
		r = c.r;
		g = c.g;
		b = c.b;
		a = c.a;
		return *this;
	}

	Color& operator=(Color&& c) noexcept
	{
		r = c.r;
		g = c.g;
		b = c.b;
		a = c.a;
		return *this;
	}

	Color& operator+=(const Color& c)
	{
		r += c.r;
		g += c.g;
		b += c.b;
		a += c.a;
		return *this;
	}

	Color& operator*=(const Color& c)
	{
		r *= c.r;
		g *= c.g;
		b *= c.b;
		a *= c.a;
		return *this;
	}

	Color& operator*=(float val)
	{
		r *= val;
		g *= val;
		b *= val;
		a *= val;
		return *this;
	}

	Color Saturated()
	{
		return Color(Math::Clamp(r, 0.f, 1.f), Math::Clamp(g, 0.f, 1.f), Math::Clamp(b, 0.f, 1.f), Math::Clamp(a, 0.f, 1.f));
	}

	Color& Saturate()
	{
		r = Math::Clamp(r, 0.f, 1.f);
		g = Math::Clamp(g, 0.f, 1.f);
		b = Math::Clamp(b, 0.f, 1.f);
		a = Math::Clamp(a, 0.f, 1.f);
		return *this;
	}

	/** \brief, set alpha to 255*/
	Color& ToRGB255()
	{
		r *= 255;
		g *= 255;
		b *= 255;
		a = 255;
		return *this;
	}

	Color& ToRGBA255()
	{
		r *= 255;
		g *= 255;
		b *= 255;
		a = 255;
		return *this;
	}

	Color RGB255()
	{
		return Color( r * 255, g * 255, b * 255, 255);
	}

	Color RGBA255()
	{
		return Color(r * 255, g * 255, b * 255, a *255);
	}

	static const Color White;
	static const Color Black;
};

inline Color operator+(float val, const Color& c)
{
	return Color(c.r + val, c.g + val, c.b + val, c.a + val);
}

inline Color operator*(float val, const Color& c)
{
	return Color(c.r * val, c.g * val, c.b * val, c.a * val);
}