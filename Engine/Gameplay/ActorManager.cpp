#include "ActorManager.h"
#include "Component.h"

ActorManager::~ActorManager()
{
	for (Actor* actor : Actors)
	{
		delete actor;
	}
}

void ActorManager::Tick(double inDeltaTime)
{
	for (Tickable* Comp : TickComponents)
	{
		Comp->Tick(inDeltaTime);
	}
}