#pragma once
#include "../Component.h"
#include "../../AssetManagement/Mesh.h"

class StaticMeshComponent : public RenderTransformComponent
{
public:
	explicit StaticMeshComponent(Mesh* inMesh = nullptr) : MeshData(inMesh) {}

	virtual void CreateRenderState() override {}
	virtual void DestroyRenderState() override {}

	void SetMesh(Mesh* inMesh) { MeshData = inMesh; MarkRenderStateDirty(); }
	Mesh* GetMesh() const { return MeshData; }

protected:
	Mesh* MeshData = nullptr;
};

