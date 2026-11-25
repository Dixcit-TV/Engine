#pragma once
#include <vector>
#include <memory>
#include "../Math/Vector.h"
#include "../Renderer/RenderResource.h"

struct Vertex
{
	Vector3 Position;
	Vector3 Normal;
	Vector4 Color;
};

struct MeshRenderResource
{
	RenderResource* VertexBuffer = nullptr;
	RenderResource* IndexBuffer = nullptr;
	RenderResource* VertexUploadBuffer = nullptr;
	RenderResource* IndexUploadBuffer = nullptr;
	uint32_t VertexCount = 0;
	uint32_t IndexCount = 0;

	~MeshRenderResource()
	{
		if (VertexBuffer)
			delete VertexBuffer;

		if (IndexBuffer)
			delete IndexBuffer;

		if (VertexUploadBuffer)
			delete VertexUploadBuffer;

		if (IndexUploadBuffer)
			delete IndexUploadBuffer;
	}
};

class Mesh
{
public:
	explicit Mesh(const std::vector<Vertex>& inVertices, const std::vector<uint32_t>& inIndices) : Vertices(inVertices), Indices(inIndices) { BuildRenderResources(); }
	explicit Mesh(std::vector<Vertex>&& inVertices, std::vector<uint32_t>&& inIndices) : Vertices(std::move(inVertices)), Indices(std::move(inIndices)) { BuildRenderResources(); }

	~Mesh();

	MeshRenderResource* GetRenderData() const { return RenderData; }

private:
	std::vector<Vertex> Vertices;
	std::vector<uint32_t> Indices;

	bool bRecreateRenderResource = false;

	MeshRenderResource* RenderData;

	void BuildRenderResources();
};

