#include "Camera.h"
#include "Structures/Ray.h"
#include "Structures/Options.h"

Camera::Camera()
{
}


Camera::~Camera()
{
}

void Camera::GetCameraRay(size_t x, size_t y, const Options& options, Ray& ray) const
{
	float xPos = (x + .5f) * options.cInvHalfWidth - 1.f;
	float yPos = 1.f - (y + .5f) * options.cInvHalfHeight;

	xPos *= options.cAspect * options.cTanHalfFov;
	yPos *= options.cTanHalfFov;

	ray.orig = m_Position;
	ray.dir = Vec3(xPos, yPos, -1.f).Normalize();
}

void Camera::GetCameraRay(size_t x, size_t y, size_t xAA, size_t yAA, const Options& options, Ray& ray) const
{
	float xPos = (x + xAA * options.cAAOffset) * options.cInvHalfWidth - 1.f;
	float yPos = 1.f - (y + yAA * options.cAAOffset) * options.cInvHalfHeight;

	xPos *= options.cAspect * options.cTanHalfFov;
	yPos *= options.cTanHalfFov;

	ray.orig = m_Position;
	ray.dir = Vec3(xPos, yPos, -1.f).Normalize();
}
