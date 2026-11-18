#include "Renderer.h"
#include "../RHI/RHIInterface.h"
#include "ShaderManager.h"

Renderer::Renderer(std::unique_ptr<RHIInterface>&& inRHI)
	: Scene()
    , RHI(std::move(inRHI))
{}

void Renderer::Init(const std::vector<RenderView>& inViews)
{
	ShaderManager::Get().CompileAllShaders(RHI.get());

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