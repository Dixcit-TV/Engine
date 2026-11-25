#pragma once
#include <type_traits>
#include <vector>
#include <algorithm>
#include "Component.h"

class Actor final
{
public:
	explicit Actor() = default;

	template<typename T, typename ...Args, typename = std::enable_if_t<std::is_base_of_v<Component, T>>>
	T* AddComponent(Args&&... inArguments)
	{
		T* NewComponent = new T(std::forward<Args>(inArguments)...);
		Components.push_back(NewComponent);
		NewComponent->SetOwner(this);

		return NewComponent;
	}

	void AddComponent(Component* inComponents);
	void RemoveComponent(Component* inComponents);

	template<typename T, typename = std::enable_if_t<std::is_base_of_v<Component, T>>>
	T* GetComponent() const
	{
		auto It{ std::find_if(std::cbegin(Components), std::cend(Components), [](const Component* inComp) { return dynamic_cast<T*>(inComp); }) };
		return It != std::cend(Components) ? static_cast<T*>(*It) : nullptr;
	}

	template<typename T, typename = std::enable_if_t<std::is_base_of_v<Component, T>>>
	std::vector<T*> GetAllComponents() const
	{
		std::vector<T*> Ret;
		std::for_each(std::cbegin(Components), std::cend(Components)
			, [&Ret](const Component* inComp)
			{
				if (T* Comp = dynamic_cast<T*>(inComp))
				{
					Ret.push_back(Comp);
				}
			});

		return Ret;
	}

	const std::vector<Component*>& GetAllComponents() const { return Components; }

protected:
	std::vector<Component*> Components;

private:
	friend class ActorManager;

	virtual ~Actor();
};

