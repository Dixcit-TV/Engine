#include "Renderer.h"
#include "../RHI/RHIInterface.h"
#include "../App/WindowInterface.h"
#include "../App/Viewport.h"
#include "ShaderCache.h"

void Renderer::Init(RHIType inType, const Viewport& inViewport, const std::unique_ptr<WindowInterface>& inWindow)
{
    RHI = RHIInterface::Create(inType, inViewport, inWindow);

	ShaderCache::Get().CompileAllShaders(RHI.get());

    Scene.Init();
}

void Renderer::RenderFrame()
{
    RHI->StartFrame();

    RHI->EndFrame();
}