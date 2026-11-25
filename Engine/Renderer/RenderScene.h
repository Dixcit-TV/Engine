#pragma once
#include <vector>
#include "../App/Viewport.h"

class CameraRenderStates;
class StaticMeshRenderStates;

class RenderView
{
public:
	RenderView(const Viewport& inViewport) : Viewport(inViewport) {}

private:
	Viewport Viewport;
};

class RenderScene
{
public:
	RenderScene() = default;

	void Init() {}

	void AddCameraRenderStates(CameraRenderStates* RenderStates)
	{
		Cameras.push_back(RenderStates);
	}

	void RemoveCameraRenderStates(CameraRenderStates* RenderStates)
	{
		auto It = std::find(Cameras.begin(), Cameras.end(), RenderStates);
		if (It != Cameras.end())
		{
			std::swap(*It, Cameras.back());
			Cameras.pop_back();
		}
	}

	void AddStaticMeshRenderStates(StaticMeshRenderStates* RenderStates)
	{
		StaticMeshes.push_back(RenderStates);
	}

	void RemoveStaticMeshRenderStates(StaticMeshRenderStates* RenderStates)
	{
		auto It = std::find(StaticMeshes.begin(), StaticMeshes.end(), RenderStates);
		if (It != StaticMeshes.end())
		{
			std::swap(*It, StaticMeshes.back());
			StaticMeshes.pop_back();
		}
	}

private:
	std::vector<CameraRenderStates*> Cameras;
	std::vector<StaticMeshRenderStates*> StaticMeshes;
};

