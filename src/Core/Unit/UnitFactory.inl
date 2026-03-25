#pragma once

#include "UnitFactory.hpp"
#include "Unit.hpp"

#include <cassert>

namespace sw::core
{
	template<ComponentConcept ...C>
	requires ValidConfigConcept<C...>
	std::shared_ptr<Unit> UnitFactory::CreateUnit(TypeList<C...>, World const* world, UnitId id, std::string_view name, typename C::DataType && ...data)
	{
		auto unit = MakeSharedUnit(id, name);

		bool result = ( unit->AddComponent<C>(world, unit.get(), std::move(data)) && ... );
		assert(result && "One or more components are not added");

		return unit;
	}

	template<typename... ArgTs>
	std::shared_ptr<Unit> UnitFactory::MakeSharedUnit(ArgTs&&... arg)
	{
		struct EnableShared : Unit
		{
			EnableShared(ArgTs&&... arg) : Unit(std::forward<ArgTs>(arg)...)
			{
			}
		};

		return std::make_shared<EnableShared>(std::forward<ArgTs>(arg)...);
	}
}
