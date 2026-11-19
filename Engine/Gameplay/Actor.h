#pragma once
#include <type_traits>
#include <vector>
#include "Component.h"

class Actor final
{
public:
	explicit Actor() = default;

	template<typename T, std::enable_if_t<std::is_base_of_v<Component, T>>>
	T* AddComponent()
	{
		T* NewComponent = new T();
		Components.push_back(NewComponent);

		return NewComponent;
	}

	void AddComponent(Component* inComponents);
	void RemoveComponent(Component* inComponents);

protected:
	std::vector<Component*> Components;

private:
	friend class ActorManager;

	virtual ~Actor();
};

