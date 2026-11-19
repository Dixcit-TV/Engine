#pragma once
#include "../Component.h"
#include "../../AssetManagement/Mesh.h"

class StaticMeshComponent : public TransformComponent
{
public:
	explicit StaticMeshComponent() = default;

	void SetMesh(Mesh* inMesh)
	{
		MeshData = inMesh;
	}

protected:
	Mesh* MeshData = nullptr;
};

