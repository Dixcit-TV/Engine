#include "DevelopmentApp.h"
#include <map>
#include <string>

#include "AssetManagement/Mesh.h"
#include "Gameplay/Components/CameraComponent.h"
#include "Gameplay/Components/StaticMeshComponent.h"
#include "Gameplay/ActorManager.h"
#include "Math/MathHelpers.h"

std::map<std::string, Mesh*> Meshes;

DevelopmentApp::DevelopmentApp()
	: Application()
{
    Settings.AppName = L"Development Application";
    Settings.Viewport.Left = 300;
    Settings.Viewport.Top = 300;
    Settings.Viewport.Width = 800;
    Settings.Viewport.Height = 600;
}

void DevelopmentApp::Initialize()
{
    std::vector<Vertex> cubeVert;
    cubeVert.push_back(Vertex{ Vector3(-1.f, -1.f, -1.f), Vector3(0.f, 1.f, 0.f), Vector4::One });
    cubeVert.push_back(Vertex{ Vector3(-1.f, 1.f, -1.f), Vector3(0.f, 1.f, 0.f), Vector4::Zero });
    cubeVert.push_back(Vertex{ Vector3(1.f, 1.f, -1.f), Vector3(0.f, 1.f, 0.f), Vector4::UnitX });
    cubeVert.push_back(Vertex{ Vector3(1.f, -1.f, -1.f), Vector3(0.f, 1.f, 0.f), Vector4::UnitY });
    cubeVert.push_back(Vertex{ Vector3(-1.f, -1.f, 1.f), Vector3(0.f, 1.f, 0.f), Vector4::UnitZ });
    cubeVert.push_back(Vertex{ Vector3(-1.f, 1.f, 1.f), Vector3(0.f, 1.f, 0.f), Vector4(1.f, 1.f, 0.f, 1.f)});
    cubeVert.push_back(Vertex{ Vector3(1.f, 1.f, 1.f), Vector3(0.f, 1.f, 0.f), Vector4(0.f, 1.f, 1.f, 1.f) });
    cubeVert.push_back(Vertex{ Vector3(1.f, -1.f, 1.f), Vector3(0.f, 1.f, 0.f), Vector4(1.f, 0.f, 1.f, 1.f) });

    std::vector<uint32_t> cubeIndices;
    cubeIndices.push_back(0); cubeIndices.push_back(1); cubeIndices.push_back(2);
    cubeIndices.push_back(0); cubeIndices.push_back(2); cubeIndices.push_back(3);
    cubeIndices.push_back(4); cubeIndices.push_back(6); cubeIndices.push_back(5);
    cubeIndices.push_back(4); cubeIndices.push_back(7); cubeIndices.push_back(6);
    cubeIndices.push_back(4); cubeIndices.push_back(5); cubeIndices.push_back(1);
    cubeIndices.push_back(4); cubeIndices.push_back(1); cubeIndices.push_back(0);
    cubeIndices.push_back(3); cubeIndices.push_back(2); cubeIndices.push_back(6);
    cubeIndices.push_back(3); cubeIndices.push_back(6); cubeIndices.push_back(7);
    cubeIndices.push_back(1); cubeIndices.push_back(5); cubeIndices.push_back(6);
    cubeIndices.push_back(1); cubeIndices.push_back(6); cubeIndices.push_back(2);
    cubeIndices.push_back(4); cubeIndices.push_back(0); cubeIndices.push_back(3);
    cubeIndices.push_back(4); cubeIndices.push_back(3); cubeIndices.push_back(7);

    Meshes.emplace("Cube", new Mesh(std::move(cubeVert), std::move(cubeIndices)));

    Actor* CameraActor = ActorManager::Get().CreateActor();

    Vector3 CameraPos = Vector3(0.f, 10.f, -40.f);
    Vector3 CameraDir = VectorHelpers::Direction(CameraPos, Vector3::Zero);
    CameraActor->AddComponent<CameraComponent>(CameraPos, CameraDir, 70.f, float(Settings.Viewport.Width) / Settings.Viewport.Height, 0.001f, 100000.f);

    Actor* CubeActor = ActorManager::Get().CreateActor();
    CubeActor->AddComponent<StaticMeshComponent>(Meshes["Cube"]);
}

void DevelopmentApp::Tick(double inDeltaTime)
{
	inDeltaTime;
}

void DevelopmentApp::Shutdown()
{
}