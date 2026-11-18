#include "Application.h"
#include "Windows/Win32Window.h"
#include "../Misc/Timer.h"
#include "../RHI/DX12RHI/DX12RHI.h"

#pragma comment (lib, "d3dcompiler.lib")
#pragma comment (lib, "D3D12.lib")
#pragma comment (lib, "dxgi.lib")
#pragma comment (lib, "dxguid.lib")

void Application::Run()
{
    AppWindow = std::make_unique<Win32Window>(Settings.Viewport, Settings.AppName);

    std::unique_ptr<RHIInterface> AppRHI = std::make_unique<DX12RHI>(Settings.Viewport, AppWindow.get());
    AppRenderer = std::make_unique<Renderer>(std::move(AppRHI));
	AppRenderer->Init({ RenderView(Settings.Viewport) });

    static auto FPSCallback = [this](UINT inFPS, double inFrameTime) 
        {  
            std::wstring windowText = AppWindow->GetName() + L" - FPS: " + std::to_wstring(inFPS) + L" - ms: " + std::to_wstring(inFrameTime);
            AppWindow->SetName(windowText);
        };

    typedef void (*CallbackWrapper)(UINT, double);
    CallbackWrapper Callback = [](UINT inFPS, double inFrameTime) { return FPSCallback(inFPS, inFrameTime); };

    FPSCounter FrameCounter(Callback);
    Timer GameTimer;
    GameTimer.Reset();

    Initialize();

    while (AppWindow->PollWindowEvents() != WindowEvents::Quit)
    {
        GameTimer.Tick();
        const double deltaT = GameTimer.GetDeltaTime();
        FrameCounter.Update(deltaT);

        Tick(deltaT);

        AppRenderer->RenderFrame();
    }

    Shutdown();
}