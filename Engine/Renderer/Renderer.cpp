#include "Renderer.h"
#include "../RHI/RHIInterface.h"

Renderer::Renderer(std::unique_ptr<RHIInterface>&& inRHI)
	: Scene()
    , RHI(std::move(inRHI))
{}

void Renderer::Init(const std::vector<RenderView>& inViews)
{
    Scene.Init();

    for (const RenderView& View : inViews)
    {
		Scene.AddView(View);
    }
}

void Renderer::RenderFrame()
{
    RHI->StartFrame();

    RHI->EndFrame();
}