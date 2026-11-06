#pragma once
#include <string>

enum class WindowEvents
{
	None
	, Quit
};

class WindowInterface
{
public:
	virtual ~WindowInterface() = default;

	virtual void* GetHandle() const = 0;
	virtual const std::wstring& GetName() const = 0;
	virtual void SetName(const std::wstring& inName) = 0;
	virtual WindowEvents PollWindowEvents() = 0;
};