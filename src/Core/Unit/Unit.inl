#pragma once

#include "Unit.hpp"

#include <cassert>

namespace sw::core
{
	template<BasicComponentConcept C>
	C* Unit::GetComponent() const noexcept
	{
		if (auto it = _components.find(GetComponentId<C>()); it != _components.end())
		{
			return static_cast<C*>(it->second.get());
		}

		return nullptr;
	}

	template<BasicComponentConcept C, typename ...ArgTs>
	bool Unit::AddComponent(ArgTs&& ...arg)
	{
		auto id = GetComponentId<C>();

		if (auto [it, result] = _components.try_emplace(id, nullptr); result)
		{
			it->second = std::make_unique<C>(std::forward<ArgTs>(arg)...);

			if constexpr (StepComponentConcept<C>)
			{
				_stepComponents.push_back(static_cast<IStepComponent*>(static_cast<C*>(it->second.get())));
			}

			return true;
		}

		return false;
	}

	template<typename T>
	Unit::ComponentId Unit::GetComponentId() noexcept
	{
		static ComponentId id = _nextComponentId++;
		assert((id < INVALID_COMPONENT_ID) && "ComponentId is invalid");
		return id;
	}
}
