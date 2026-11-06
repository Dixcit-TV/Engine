#include "Win32Window.h"

#include <windowsx.h>

#include "../Viewport.h"

Win32Window::Win32Window(const Viewport& inViewport, const std::wstring& inName)
    : HWnd(nullptr)
    , WindowName(inName)
{
#if defined(DEBUG) || defined(_DEBUG)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    WNDCLASSEX wc;

    ZeroMemory(&wc, sizeof(WNDCLASSEX));

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandleA(0);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.lpszClassName = L"WindowClass";

    RegisterClassEx(&wc);

    HWnd = CreateWindowEx(NULL,
        L"WindowClass",
        inName.c_str(),
        WS_OVERLAPPEDWINDOW,
        inViewport.Left, inViewport.Top,
        inViewport.Width, inViewport.Height,
        NULL,
        NULL,
        wc.hInstance,
        NULL);

    ShowWindow(HWnd, SW_SHOWNORMAL);
}

LRESULT CALLBACK Win32Window::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    //case WM_MOUSEMOVE:
    //    OnMouseMove(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
    //    break;
    //case WM_KEYDOWN:
    //    std::cout << "WM_KEYDOWN %i %i" << wParam << (lParam & 30) << std::endl;
    //    OnKeyDown(wParam);
    //    break;
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}

WindowEvents Win32Window::PollWindowEvents()
{
    MSG msg{};

    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.message == WM_QUIT ? WindowEvents::Quit : WindowEvents::None;
}