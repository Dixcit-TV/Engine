#pragma once
#include <memory>
#include "Viewport.h"
#include "WindowInterface.h"
#include "../Renderer/Renderer.h"

struct AppSettings
{
	const wchar_t* AppName = nullptr;
	Viewport Viewport;
};

class Application
{
public:
	Application() : Settings(), AppWindow(nullptr), AppRenderer(nullptr) {};
	virtual ~Application() = default;

	void Run();

protected:
	virtual void Initialize() = 0;
	virtual void Tick(double inDeltaTime) = 0;
	virtual void Shutdown() = 0;

	AppSettings Settings;
	std::unique_ptr<WindowInterface> AppWindow;
	std::unique_ptr<Renderer> AppRenderer;
};

