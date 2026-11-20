#include "Component.h"
#include "ActorManager.h"

Tickable::~Tickable()
{
	SetTickEnabled(false);
}

void Tickable::SetTickEnabled(bool inEnabled)
{
	if (inEnabled != bIsTickEnabled)
	{
		bIsTickEnabled = inEnabled;

		if (bIsTickEnabled)
		{
			ActorManager::Get().RegisterTick(this);
		}
		else
		{
			ActorManager::Get().UnregisterTick(this);
		}
	}
}

Renderable::~Renderable()
{
	if (bIsDirty)
	{
		ActorManager::Get().UnregisterDirtyRenderComponent(this);
		bIsDirty = false;
	}
}

void Renderable::MarkRenderStateDirty()
{
	if (!bIsDirty)
	{
		ActorManager::Get().RegisterDirtyRenderComponent(this);
		bIsDirty = true;
	}
}