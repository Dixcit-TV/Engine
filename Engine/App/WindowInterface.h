#pragma once
#include <cassert>
#include <string>

enum class WindowEvents
{
	None
	, Quit
};

class WindowInterface
{
public:
	virtual ~WindowInterface() { HasInstance = false; }
	WindowInterface(const WindowInterface& other) = delete;
	WindowInterface(WindowInterface&& other) = delete;
	WindowInterface& operator=(const WindowInterface& other) = delete;
	WindowInterface& operator=(WindowInterface&& other) = delete;

	virtual void* GetHandle() const = 0;
	virtual const std::wstring& GetName() const = 0;
	virtual void SetName(const std::wstring& inName) = 0;
	virtual WindowEvents PollWindowEvents() = 0;

	template<typename T, typename ...Args, typename = std::enable_if_t<std::is_base_of_v<WindowInterface, T>>>
	static T* Create(Args&&... inArguments)
	{
		assert(!HasInstance && "WindowInterface already created. Application cannot have more than one.");
		return new T(std::forward<Args>(inArguments)...);
	}

protected:
	explicit WindowInterface() { HasInstance = true; }

private:
	inline static bool HasInstance = false;
};