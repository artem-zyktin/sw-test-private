#pragma once

#include "UnitId.hpp"
#include "Core/Components.hpp"

#include <string>
#include <string_view>
#include <cstddef>
#include <limits>
#include <unordered_map>
#include <memory>
#include <vector>

namespace sw::core
{
	struct Unit
	{
		UnitId GetId() const noexcept;
		std::string_view GetName() const noexcept;

		template<BasicComponentConcept C>
		C* GetComponent() const noexcept;

		void Step();

		bool operator==(const Unit& other) const noexcept;

	protected:
		friend struct UnitFactory;

		Unit(UnitId id, std::string_view name);

		template<BasicComponentConcept C, typename... ArgTs>
		bool AddComponent(ArgTs&&... arg);

	private:
		using ComponentId = uint32_t;
		static constexpr ComponentId INVALID_COMPONENT_ID = std::numeric_limits<ComponentId>::max();

		static ComponentId _nextComponentId;

		UnitId _id;
		std::string _name;
		std::unordered_map<ComponentId, std::unique_ptr<IComponent>> _components;
		std::vector<IStepComponent*> _stepComponents;

		template<typename T>
		static ComponentId GetComponentId() noexcept;
	};
}

#include "Unit.inl"
