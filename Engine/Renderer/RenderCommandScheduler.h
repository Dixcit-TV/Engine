#pragma once
#include <functional>
#include "RenderScene.h"

class RHIInterface;

class RenderCommandScheduler
{
	typedef std::function<void(RHIInterface*, RenderScene&)> CommandType;

public:
	inline static void Enqueue(CommandType&& inCommand)
	{
		Commands.push_back(std::move(inCommand));
	}

	inline static void Enqueue(const CommandType& inCommand)
	{
		Commands.push_back(inCommand);
	}

private:
	friend class Renderer;

	inline static void Submit(RHIInterface* inRHI, RenderScene& inRenderScene)
	{
		for (const CommandType& Command : Commands)
		{
			Command(inRHI, inRenderScene);
		}
		Commands.clear();
	}

	inline static std::vector<CommandType> Commands;
};