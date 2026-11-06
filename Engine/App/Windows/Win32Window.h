#pragma once
#include <Windows.h>
#include <string>
#include "../WindowInterface.h"

struct Viewport;

class Win32Window final : public WindowInterface
{
public:
	Win32Window(const Viewport& inViewport, const std::wstring& inName);

	void* GetHandle() const override { return static_cast<void*>(HWnd); }
	const std::wstring& GetName() const override { return WindowName; }
	void SetName(const std::wstring& inName) override { SetWindowText(HWnd, inName.c_str()); }

	WindowEvents PollWindowEvents() override;

private:
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	HWND HWnd;
	std::wstring WindowName;
};

