#pragma once
#include "../Misc/Singleton.h"
#include "Actor.h"
#include <map>

class Tickable;
class Renderable;

class ActorManager : public Singleton<ActorManager>
{
public:
	virtual ~ActorManager() override;
	ActorManager(const ActorManager& other) = delete;
	ActorManager(ActorManager&& other) = delete;
	ActorManager& operator=(const ActorManager& other) = delete;
	ActorManager& operator=(ActorManager&& other) = delete;

	inline static Actor* CreateActor()
	{
		Actor* NewActor = new Actor();
		Get().RegisterActor(NewActor);

		return NewActor;
	}

	inline void RegisterActor(Actor* inActor)
	{
		Actors.push_back(inActor);
	}

	inline void UnregisterActor(Actor* inActor)
	{
		auto It = std::find(Actors.begin(), Actors.end(), inActor);
		if (It != Actors.end())
		{
			delete (*It);
			std::swap(*It, Actors.back());
			Actors.pop_back();
		}
	}

	inline const std::vector<Actor*>& GetActors() const { return Actors; }

	inline void RegisterTick(Tickable* inTickComponent)
	{
		TickComponents.push_back(inTickComponent);
	}

	inline void UnregisterTick(Tickable* inTickComponent)
	{
		auto It = std::find(TickComponents.begin(), TickComponents.end(), inTickComponent);
		if (It != TickComponents.end())
		{
			std::swap(*It, TickComponents.back());
			TickComponents.pop_back();
		}
	}

	inline void RegisterDirtyRenderComponent(Renderable* inRenderComponent)
	{
		DirtyComponents.push_back(inRenderComponent);
	}

	inline void UnregisterDirtyRenderComponent(Renderable* inRenderComponent)
	{
		auto It = std::find(DirtyComponents.begin(), DirtyComponents.end(), inRenderComponent);
		if (It != DirtyComponents.end())
		{
			std::swap(*It, DirtyComponents.back());
			DirtyComponents.pop_back();
		}
	}

	void Tick(double inDeltaTime);
	void UpdateRenderStates();

protected:
	friend class Singleton<ActorManager>;

	explicit ActorManager() = default;

private:
	std::vector<Actor*> Actors;
	std::vector<Tickable*> TickComponents;
	std::vector<Renderable*> DirtyComponents;
};

