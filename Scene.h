#pragma once
#include "Camera.h"
#include <vector>

struct Object;
struct Light;

class Scene
{
public:
	Scene();
	~Scene();

	void SetCamera(Camera* pCamera) { m_pCamera = pCamera; }
	Camera* GetCamera() const { return m_pCamera; }

	void AddObject(Object* pObject) { m_pObjects.push_back(pObject); }
	std::vector<Object*>& GetObjects() { return m_pObjects; }

	void AddLight(Light* light) { m_pLights.push_back(light); }
	std::vector<Light*>& GetLights() { return m_pLights; }

	void SetAmbientLight(Light* pAmbient) { m_pAmbient = pAmbient; }
	Light* GetAmbientLight() { return m_pAmbient; }

private:
	Camera* m_pCamera = nullptr;

	std::vector<Object*> m_pObjects;
	std::vector<Light*> m_pLights;
	Light* m_pAmbient = nullptr;
};

