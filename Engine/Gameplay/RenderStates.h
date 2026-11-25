#pragma once
#include "../Math/Matrix.h"

class Mesh;
class RenderResource;
struct MeshRenderResource;
class StaticMeshComponent;
class CameraComponent;

class CameraRenderStates
{
public:
	CameraRenderStates(const CameraComponent* inComponent);

private:
	Matrix44 View;
	Matrix44 Projection;
	Matrix44 ViewProjection;
};

class StaticMeshRenderStates
{
public:
	StaticMeshRenderStates(const StaticMeshComponent* inComponent);

private:
	Matrix44 World;
	MeshRenderResource* MeshRenderData;
};