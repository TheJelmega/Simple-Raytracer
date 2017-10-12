#pragma once
#include "Math.h"

struct Vec3;
inline Vec3 operator*(float val, const Vec3& v);

struct Vec3
{
	float x, y, z;

	Vec3()
		: x(0.f), y(0.f), z(0.f)
	{}

	Vec3(float val)
		: x(val), y(val), z(val)
	{}

	Vec3(float x, float y, float z)
		: x(x), y(y), z(z)
	{}

	Vec3(const Vec3& v)
		: x(v.x), y(v.y), z(v.z)
	{}

	Vec3(Vec3&& v) noexcept
		: x(v.x), y(v.y), z(v.z)
	{
	}

	Vec3 operator+() const
	{
		return *this;
	}

	Vec3 operator-() const
	{
		return Vec3(-x, -y, -z);
	}

	Vec3 operator+(Vec3&& v) const
	{
		return Vec3(x + v.x, y + v.y, z + v.z);
	}

	inline Vec3 operator+(const Vec3& v) const
	{
		return Vec3(x + v.x, y + v.y, z + v.z);
	}

	inline Vec3 operator-(const Vec3& v) const
	{
		return Vec3(x - v.x, y - v.y, z - v.z);
	}

	inline Vec3 operator*(const Vec3& v) const
	{
		return Vec3(x * v.x, y * v.y, z * v.z);
	}

	inline Vec3 operator/(const Vec3& v) const
	{
		return Vec3(x / v.x, y / v.y, z / v.z);
	}

	inline Vec3 operator+(float val) const
	{
		return Vec3(x + val, y + val, z + val);
	}

	inline Vec3 operator-(float val) const
	{
		return Vec3(x - val, y - val, z - val);
	}

	inline Vec3 operator*(float val) const
	{
		return Vec3(x * val, y * val, z * val);
	}

	inline Vec3 operator/(float val) const
	{
		val = 1.f / val;
		return Vec3(x * val, y * val, z * val);
	}

	inline Vec3& operator=(const Vec3& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		return *this;
	}

	inline Vec3& operator=(Vec3&& v) noexcept
	{
		//std::swap(_v, v._v);
		x = v.x;
		y = v.y;
		z = v.z;
		return *this;
	}

	inline Vec3& operator+=(const Vec3& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

	inline Vec3& operator-=(const Vec3& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}

	inline Vec3& operator*=(const Vec3& v)
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;
		return *this;
	}

	inline Vec3& operator/=(const Vec3& v)
	{
		x /= v.x;
		y /= v.y;
		z /= v.z;
		return *this;
	}

	inline Vec3& operator+=(float val)
	{
		x += val;
		y += val;
		z += val;
		return *this;
	}

	inline Vec3& operator-=(float val)
	{
		x -= val;
		y -= val;
		z -= val;
		return *this;
	}

	inline Vec3& operator*=(float val)
	{
		x *= val;
		y *= val;
		z *= val;
		return *this;
	}

	inline Vec3& operator/=(float val)
	{
		val = 1.f / val;
		x *= val;
		y *= val;
		z *= val;
		return *this;
	}

	inline float SqLength() const
	{
		return x * x + y * y + z * z;
	}

	inline float Length() const
	{
		return sqrt(x * x + y * y + z * z);
	}

	inline float Dot(const Vec3& v) const
	{
		return x * v.x + y * v.y + z * v.z;
	}

	inline Vec3 Cross(const Vec3& v) const
	{
		return Vec3(
			y * v.z - z * v.y,
			z * v.x - x * v.z,
			x * v.y - y * v.x
		);
	}

	inline Vec3 Normalized() const
	{
		float l = SqLength();
		if (l < Math::Epsilon)
			return *this;

		l = 1.f / sqrt(l);
		return Vec3(x * l, y * l, z * l);
	}

	inline Vec3& Normalize()
	{
		float l = SqLength();
		if (l < Math::Epsilon)
			return *this;

		l = 1.f / sqrt(l);
		x *= l;
		y *= l;
		z *= l;
		return *this;
	}

	inline Vec3 Reflected(const Vec3& normal) const
	{
		float dot = Dot(normal);
		return *this - 2.f * dot * normal;
	}

	inline Vec3& Reflect(const Vec3& normal)
	{
		float dot = Dot(normal);
		*this -= 2.f * dot * normal;
		return *this;
	}

	inline Vec3 Refracted(const Vec3& normal, float n1, float n2)
	{
		float cosi = Math::Clamp(Dot(normal), -1.f, 1.f);
		Vec3 n = normal;
		if (cosi < 0)
		{
			cosi = -cosi;
		}
		else
		{
			Math::Swap(n1, n2);
			n = -normal;
		}
		float eta = n1 / n2;
		float k = 1.f - eta * eta * (1.f - cosi * cosi);
		return k < Math::Epsilon ? Vec3() : eta * *this + (eta * cosi - sqrt(k)) * n;
	}

	inline Vec3& Refract(const Vec3& normal, float n1, float n2)
	{
		float cosi = Math::Clamp(Dot(normal), -1.f, 1.f);
		Vec3 n = normal;
		if (cosi < 0)
		{
			cosi = -cosi;
		}
		else
		{
			Math::Swap(n1, n2);
			n = -normal;
		}
		float eta = n1 / n2;
		float k = 1.f - eta * eta * (1.f - cosi * cosi);
		if (k >= Math::Epsilon)
		{
			*this *= eta;
			*this += (eta * cosi - sqrt(k)) * n;
		}
		else
		{
			*this = Vec3();
		}
		return *this;
	}

	inline bool isInside(const Vec3& min, const Vec3& max) const
	{
		if (x < min.x || x > max.x)
			return false;
		if (y < min.y || y > max.y)
			return false;
		if (z < min.z || z > max.z)
			return false;
		return true;
	}
};

inline Vec3 operator*(float val, const Vec3& v)
{
	return Vec3(v.x * val, v.y * val, v.z * val);
}
