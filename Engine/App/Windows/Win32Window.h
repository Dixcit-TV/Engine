#pragma once
#include <Windows.h>
#include <string>
#include "../WindowInterface.h"

struct Viewport;

class Win32Window final : public WindowInterface
{
public:
	virtual ~Win32Window() override = default;
	Win32Window(const Win32Window& other) = delete;
	Win32Window(Win32Window&& other) = delete;
	Win32Window& operator=(const Win32Window& other) = delete;
	Win32Window& operator=(Win32Window&& other) = delete;

	void* GetHandle() const override { return static_cast<void*>(HWnd); }
	const std::wstring& GetName() const override { return WindowName; }
	void SetName(const std::wstring& inName) override { SetWindowText(HWnd, inName.c_str()); }

	WindowEvents PollWindowEvents() override;

private:
	friend WindowInterface;

	Win32Window(const Viewport& inViewport, const std::wstring& inName);

	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	HWND HWnd;
	std::wstring WindowName;
};

