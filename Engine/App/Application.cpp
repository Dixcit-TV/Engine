#include "Application.h"
#include "Windows/Win32Window.h"
#include "../Misc/Timer.h"
#include "../RHI/RHIInterface.h"
#include "../Gameplay/ActorManager.h"

void Application::Run()
{
    AppWindow.reset(WindowInterface::Create<Win32Window>(Settings.Viewport, Settings.AppName));

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

    AppRenderer.reset(Renderer::Create());
    AppRenderer->Init(RHIType::DX12, Settings.Viewport, AppWindow);

    Initialize();

    while (AppWindow->PollWindowEvents() != WindowEvents::Quit)
    {
        GameTimer.Tick();
        const double DeltaT = GameTimer.GetDeltaTime();
        FrameCounter.Update(DeltaT);

        Tick(DeltaT);
        ActorManager::Get().Tick(DeltaT);

        ActorManager::Get().UpdateRenderStates();

        AppRenderer->RenderFrame();
    }

    Shutdown();
}