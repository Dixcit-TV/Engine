#pragma once
#include <memory>
#include <vector>
#include "../RHI/RHIInterface.h"
#include "RenderScene.h"

class Renderer
{
public:
	explicit Renderer() = default;
	explicit Renderer(std::unique_ptr<RHIInterface>&& inRHI);

	void Init(const std::vector<RenderView>& inViews);
	void RenderFrame();
private:
	RenderScene Scene;
	std::unique_ptr<RHIInterface> RHI;
};

