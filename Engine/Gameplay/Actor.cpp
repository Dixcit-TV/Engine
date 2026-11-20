#include "Actor.h"
#include "Component.h"

Actor::~Actor()
{
	for (Component* Comp : Components)
	{
		Comp->Deinitialize();
		delete Comp;
	}
}

void Actor::AddComponent(Component* inComponents) 
{ 
	Components.push_back(inComponents); 
	inComponents->SetOwner(this);
}

void Actor::RemoveComponent(Component* inComponents)
{
	auto It = std::find(Components.begin(), Components.end(), inComponents);
	if (It != Components.end())
	{
		(*It)->Deinitialize();
		delete *It;
		std::swap(*It, Components.back());
		Components.pop_back();
	}
}