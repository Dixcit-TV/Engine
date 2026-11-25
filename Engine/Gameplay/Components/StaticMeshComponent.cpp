#include "StaticMeshComponent.h"
#include "../../Renderer/RenderCommandScheduler.h"

void StaticMeshComponent::CreateRenderState()
{
	StaticMeshRenderStates* NewRenderState = new StaticMeshRenderStates(this);
	RenderStates = NewRenderState;

	RenderCommandScheduler::Enqueue([NewRenderState](RHIInterface*, RenderScene& inScene)
		{
			inScene.AddStaticMeshRenderStates(NewRenderState);
		});
}

void StaticMeshComponent::DestroyRenderState()
{
	if (RenderStates)
	{
		RenderCommandScheduler::Enqueue([OldRenderState = std::move(RenderStates)](RHIInterface*, RenderScene& inScene)
			{
				inScene.RemoveStaticMeshRenderStates(OldRenderState);
				delete OldRenderState;
			});
	}

	RenderStates = nullptr;
}

void StaticMeshComponent::Deinitialize()
{
	DestroyRenderState();
}