#pragma once
#include <vector>
#include "../App/Viewport.h"

class RenderView
{
public:
	RenderView(const Viewport& inViewport) : Viewport(inViewport) {}

private:
	Viewport Viewport;
};

class RenderScene
{
public:
	RenderScene() = default;

	void Init() {}

	void AddView(const RenderView& inView)
	{
		Views.push_back(inView);
	}

private:
	std::vector<RenderView> Views;
};

