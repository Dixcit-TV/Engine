#pragma once
#include <vector>
#include <memory>
#include "../Math/Vector.h"

class RenderResource;

struct Vertex
{
	Vector3 Position;
	Vector3 Normal;
	Vector4 Color;
};

struct MeshRenderResource
{
	std::shared_ptr<RenderResource> VertexBuffer = nullptr;
	std::shared_ptr<RenderResource> IndexBuffer = nullptr;
	uint32_t VertexCount = 0;
	uint32_t IndexCount = 0;
};

class Mesh
{
public:
	explicit Mesh(const std::vector<Vertex>& inVertices, const std::vector<uint32_t>& inIndices) : Vertices(inVertices), Indices(inIndices) {}
	explicit Mesh(std::vector<Vertex>&& inVertices, std::vector<uint32_t>&& inIndices) : Vertices(inVertices) , Indices(inIndices) {}

private:
	std::vector<Vertex> Vertices;
	std::vector<uint32_t> Indices;

	bool bRecreateRenderResource = false;

	MeshRenderResource RenderResource;
};

