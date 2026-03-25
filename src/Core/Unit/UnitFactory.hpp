#pragma once

#include <Core/Components.hpp>
#include <Core/Utils.hpp>
#include "UnitId.hpp"

#include <memory>
#include <string_view>

namespace sw::core
{
	struct Unit;
	struct World;

	struct UnitFactory
	{
		template<ComponentConcept... C>
		requires ValidConfigConcept<C...>
		std::shared_ptr<Unit> CreateUnit(TypeList<C...>, World const* world, UnitId id, std::string_view name, typename C::DataType&&... data);

	private:
		template<typename... ArgTs>
		std::shared_ptr<Unit> MakeSharedUnit(ArgTs&&... arg);
	};
}

#include "UnitFactory.inl"
