#include "Scene.h"
#include "Object/Object.h"
#include "Structures/Light.h"

Scene::Scene()
{
}


Scene::~Scene()
{
	if (m_pCamera)
	{
		delete m_pCamera;
		m_pCamera = nullptr;
	}

	for (Object* pObject : m_pObjects)
	{
		if (pObject)
			delete pObject;
	}
	m_pObjects.clear();

	for (Light* pLight : m_pLights)
	{
		if (pLight)
			delete pLight;
	}
	m_pLights.clear();

	if (m_pAmbient)
	{
		delete m_pAmbient;
		m_pAmbient = nullptr;
	}
}
