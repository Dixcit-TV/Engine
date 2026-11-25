#include "ActorManager.h"
#include "Component.h"

void ActorManager::Shutdown()
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

void ActorManager::UpdateRenderStates()
{
	for (Renderable* Comp : DirtyComponents)
	{
		Comp->DestroyRenderState();
		Comp->CreateRenderState();
		Comp->MarkRenderStateValid();
	}

	DirtyComponents.clear();
}