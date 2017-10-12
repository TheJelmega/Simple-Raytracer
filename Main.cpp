#include <chrono>
#include <iostream>
#include "Includes.h"
#include "Raytracer.h"
#include "Utilities/Mesh.h"
#include "Utilities/ObjLoader.h"
#include "Model.h"

// Memory leak check
#if defined(_DEBUG) || defined (DEBUG)
#include <Windows.h>
#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h> 
#include <crtdbg.h>
#endif

int main(int argc, char* argv[])
{
	// Memory leak check
#if defined(_DEBUG) || defined (DEBUG)
	UNREFERENCED_PARAMETER(argc);
	UNREFERENCED_PARAMETER(argv);
	HeapQueryInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0, NULL);
	_CrtDumpMemoryLeaks();
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(0);
#endif

	std::cout << "Starting Raytracer\n";
	auto start = std::chrono::high_resolution_clock::now();

	Options options;
	options.width = 1024;
	options.height = 1024;
	options.fov = Math::ToRadians(60.f);
	options.traceDepth = 8;
	options.antiAlias = 8;
	options.ior = 1.000299f;
	options.shadowBias = 0.001f;
	//options.cullTriangle = false;

	options.Calculate();

	Image* pImg = new Image(options.width, options.height);

	Scene* pScene = new Scene();
	Camera* pCam = new Camera();
	pCam->SetPosition(Vec3(0.f, 0.f, 100.f));

	pScene->SetCamera(pCam);

	//Scene objects
	Material mat;
	mat.color = { .8f, .8f, .8f };
#if 1
	//mat.reflectiveness = .1f;
	pScene->AddObject(new Plane(Vec3(0.f, -50.f, 0.f), Vec3(0.f, 1.f, 0.f), mat));
	//mat.reflectiveness = 0.f;
	pScene->AddObject(new Plane(Vec3(0.f, 50.f, 0.f), Vec3(0.f, -1.f, 0.f), mat));
	pScene->AddObject(new Plane(Vec3(0.f, 0.f, -50.f), Vec3(0.f, 0.f, 1.f), mat));
	mat.color = { .8f, .4f, .4f };
	pScene->AddObject(new Plane(Vec3(-50.f, 0.f, 0.f), Vec3(1.f, 0.f, 0.f), mat));
	mat.color = { .4f, .8f, .4f };
	pScene->AddObject(new Plane(Vec3(50.f, 0.f, 0.f), Vec3(-1.f, 0.f, 0.f), mat));
	mat.color = { .2f, .2f, .2f };
	//mat.reflectiveness = .1f;
	pScene->AddObject(new Plane(Vec3(0.f, 0.f, 50.f), Vec3(0.f, 0.f, -1.f), mat));

	mat.roughness = .5f;
	mat.shininess = 16.f;
	mat.reflectiveness = .8f;
	pScene->AddObject(new Sphere(Vec3(-23, -33.5, -28), 16.5f, mat));

	mat.roughness = 1.f;
	mat.reflectiveness = 0.f;
	mat.transparency = 1.f;
	mat.ior = 4.05f;
	pScene->AddObject(new Sphere(Vec3(23, -33.5, 18), 16.5f, mat));
#endif

	auto pLight = new Light();
	pLight->pos = { 0.f, 49.f, 0.f };
	pLight->intensity = .8f;
	pScene->AddLight(pLight);

	/*pLight = new Light();
	pLight->pos = { 0.f, -10.f, 0.f };
	pLight->intensity = .2f;
	pScene->AddLight(pLight);*/

	pLight = new Light();
	pLight->intensity = .2f;
	pScene->SetAmbientLight(pLight);

#if 0
	mat.color = {.8f, .7f, .6f};
	Mesh* pMesh = ObjLoader::Load("stanford bunny.obj");
	auto pModel = new Model(Vec3(0.f, -50.f, -50.f), 500.f, mat, 3);
	pModel->Init(pMesh);
	delete pMesh;
	pMesh = 0;
	pScene->AddObject(pModel);

	mat.color = { .5f, .5f, .5f };
	mat.reflectiveness = 0.5f;
	pScene->AddObject(new Plane(Vec3(0.f, -33.f, 50.f), Vec3(0.f, 1.f, 0.f), mat));
#endif

	Raytracer raytracer;

#pragma region Time_Setup
	auto end = std::chrono::high_resolution_clock::now();
	auto dur = end - start;
	std::cout << "Setup took " << std::chrono::duration_cast<std::chrono::milliseconds>(dur).count() << " miliseconds\n";
	start = std::chrono::high_resolution_clock::now();
#pragma endregion 

	raytracer.Trace(options, pScene, pImg);

#pragma region Time_Trace
	end = std::chrono::high_resolution_clock::now();
	dur = end - start;
	std::cout << "Raytracing took " << std::chrono::duration_cast<std::chrono::milliseconds>(dur).count() << " miliseconds\n";
	start = std::chrono::high_resolution_clock::now();
#pragma endregion 

	pImg->Save("renders/render.png");

#pragma region Time_Save
	end = std::chrono::high_resolution_clock::now();
	dur = end - start;
	std::cout << "Saving image took " << std::chrono::duration_cast<std::chrono::milliseconds>(dur).count() << " miliseconds\n";
#pragma endregion 

	delete pImg;
	delete pScene;

	std::cin.get();
	return 0;
}
