#pragma once
#include "Math/Vec3.h"

struct Ray;
struct Options;

class Camera
{
public:
	Camera();
	~Camera();

	void SetPosition(const Vec3& position) { m_Position = position; }

	void GetCameraRay(size_t x, size_t y, const Options& options, Ray& ray) const;
	void GetCameraRay(size_t x, size_t y, size_t xAA, size_t yAA, const Options& options, Ray& ray) const;

private:
	Vec3 m_Position;
};

