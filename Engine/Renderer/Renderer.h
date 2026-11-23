#pragma once
#include <memory>
#include <cassert>
#include "RenderScene.h"
#include "../RHI/RHIInterface.h"

class WindowInterface;
enum class RHIType;
struct Viewport;

class Renderer
{
public:
	~Renderer() { HasInstance = false; }
	Renderer(const Renderer& other) = delete;
	Renderer(Renderer&& other) = delete;
	Renderer& operator=(const Renderer& other) = delete;
	Renderer& operator=(Renderer&& other) = delete;

	void Init(RHIType inType, const Viewport& inViewport, const std::unique_ptr<WindowInterface>& inWindow);
	void RenderFrame();

	static Renderer* Create() 
	{  
		assert(!HasInstance && "Renderer already created. Application cannot have more than one.");
		return new Renderer();
	}

protected:
	explicit Renderer() : Scene(), RHI(nullptr) { HasInstance = true; }

private:
	RenderScene Scene;
	std::unique_ptr<RHIInterface> RHI;

	inline static bool HasInstance = false;
};

