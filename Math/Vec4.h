#pragma once

struct Vec4
{
	float x, y, z, w;

	Vec4()
		: x(0.f), y(0.f), z(0.f), w(1.f)
	{}

	Vec4(float x, float y, float z)
		: x(x), y(y), z(z), w(1.f)
	{}

	Vec4(float x, float y, float z, float w)
		: x(x), y(y), z(z), w(w)
	{}

	Vec4(const Vec4& v)
		: x(v.x), y(v.y), z(v.z), w(v.w)
	{}

	Vec4(Vec4&& v) noexcept
		: x(v.x), y(v.y), z(v.z), w(v.w)
	{}

	Vec4& operator=(const Vec4& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		w = v.w;
		return *this;
	}

	Vec4& operator=(Vec4&& v) noexcept
	{
		x = v.x;
		y = v.y;
		z = v.z;
		w = v.w;
		return *this;
	}


	inline float Dot(const Vec4& v) const
	{
		return x * v.x + y * v.y + z * v.z + w * v.w;
	}
};
