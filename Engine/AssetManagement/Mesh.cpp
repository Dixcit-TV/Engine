#include "Mesh.h"
#include "../RHI/RHIInterface.h"
#include "../RHI/RHIResource.h"
#include "../Renderer/RenderCommandScheduler.h"
#include "../Renderer/RenderScene.h"

Mesh::~Mesh()
{
	if (RenderData)
	{
		RenderCommandScheduler::Enqueue([OldRenderData = std::move(RenderData)](RHIInterface*, RenderScene&)
			{
				delete OldRenderData;
			});

		RenderData = nullptr;
	}
}

void Mesh::BuildRenderResources()
{
	MeshRenderResource* NewRenderData = new MeshRenderResource();
	RenderData = NewRenderData;

	RenderCommandScheduler::Enqueue([NewRenderData, inVertices = Vertices, inIndices = Indices](RHIInterface* inRHI, RenderScene&)
		{
			RHIResourceDesc BufferDesc = RHIResourceDesc::Buffer((uint32_t)inVertices.size(), sizeof(Vertex));
			NewRenderData->VertexBuffer = inRHI->CreateCommittedResource(BufferDesc, &NewRenderData->VertexUploadBuffer, (void*)inVertices.data());

			BufferDesc = RHIResourceDesc::Buffer((uint32_t)inIndices.size(), sizeof(uint32_t));
			NewRenderData->IndexBuffer = inRHI->CreateCommittedResource(BufferDesc, &NewRenderData->IndexUploadBuffer, (void*)inVertices.data());
		});
}