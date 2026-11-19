#include "Component.h"
#include "ActorManager.h"

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