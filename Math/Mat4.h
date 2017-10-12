#pragma once
#include "Vec3.h"
#include "Vec4.h"
#include <algorithm>

struct Mat4
{
	union
	{
		struct
		{
			float m00, m01, m02, m03;
			float m10, m11, m12, m13;
			float m20, m21, m22, m23;
			float m30, m31, m32, m33;
		};
		struct { Vec4 v0, v1, v2, v3; };
		float _m[16];
	};

	Mat4()
		: m00(1.f), m01(0.f), m02(0.f), m03(0.f)
		, m10(0.f), m11(1.f), m12(0.f), m13(0.f)
		, m20(0.f), m21(0.f), m22(1.f), m23(0.f)
		, m30(0.f), m31(0.f), m32(0.f), m33(1.f)
	{}

	Mat4(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33)
		: m00(m00), m01(m01), m02(m02), m03(m03)
		, m10(m10), m11(m11), m12(m12), m13(m13)
		, m20(m20), m21(m21), m22(m22), m23(m23)
		, m30(m30), m31(m31), m32(m32), m33(m33)
	{}

	Mat4(const Mat4& m)
		: m00(m.m00), m01(m.m01), m02(m.m02), m03(m.m03)
		, m10(m.m10), m11(m.m11), m12(m.m12), m13(m.m13)
		, m20(m.m20), m21(m.m21), m22(m.m22), m23(m.m23)
		, m30(m.m30), m31(m.m31), m32(m.m32), m33(m.m33)
	{}

	Mat4(Mat4&& m) noexcept
	{
		std::swap(_m, m._m);
	}

	Mat4 operator*(const Mat4& m) const
	{
		Vec4 h0(m.m00, m.m10, m.m20, m.m30);
		Vec4 h1(m.m01, m.m11, m.m21, m.m31);
		Vec4 h2(m.m02, m.m12, m.m22, m.m32);
		Vec4 h3(m.m03, m.m13, m.m23, m.m33);

		float t00 = v0.Dot(h0);
		float t01 = v0.Dot(h1);
		float t02 = v0.Dot(h2);
		float t03 = v0.Dot(h3);
		float t10 = v1.Dot(h0);
		float t11 = v1.Dot(h1);
		float t12 = v1.Dot(h2);
		float t13 = v1.Dot(h3);
		float t20 = v2.Dot(h0);
		float t21 = v2.Dot(h1);
		float t22 = v2.Dot(h2);
		float t23 = v2.Dot(h3);
		float t30 = v3.Dot(h0);
		float t31 = v3.Dot(h1);
		float t32 = v3.Dot(h2);
		float t33 = v3.Dot(h3);

		return Mat4(t00, t01, t02, t03, t10, t11, t12, t13, t20, t21, t22, t23, t30, t31, t32, t33);
	}

	Mat4& operator=(const Mat4& m)
	{
		m00 = m.m00;
		m01 = m.m01;
		m02 = m.m02;
		m03 = m.m03;
		m10 = m.m10;
		m11 = m.m11;
		m12 = m.m12;
		m13 = m.m13;
		m20 = m.m20;
		m21 = m.m21;
		m22 = m.m22;
		m23 = m.m23;
		m30 = m.m30;
		m31 = m.m31;
		m32 = m.m32;
		m33 = m.m33;
		return *this;
	}

	Mat4& operator=(Mat4&& m) noexcept
	{
		std::swap(_m, m._m);
		return *this;
	}

	Mat4& ToScaleTranslation(const Vec3& position, float scale)
	{
		m00 = m11 = m22 = scale;
		m30 = position.x;
		m31 = position.y;
		m32 = position.z;
		return *this;
	}
};

inline Vec4 operator*(const Vec4& v, const Mat4& m)
{
	Vec4 c0(m.m00, m.m10, m.m20, m.m30);
	Vec4 c1(m.m01, m.m11, m.m21, m.m31);
	Vec4 c2(m.m02, m.m12, m.m22, m.m32);
	Vec4 c3(m.m03, m.m13, m.m23, m.m33);

	float tx = v.Dot(c0);
	float ty = v.Dot(c1);
	float tz = v.Dot(c2);
	float tw = v.Dot(c3);
	return Vec4(tx, ty, tz, tw);
}
