#pragma once

#include "Core.hpp"
#include "Features/Data.hpp"

#include <memory>
#include <string_view>

namespace sw
{
	struct UnitConfigRegistry
	{
		UnitConfigRegistry(std::unique_ptr<core::UnitFactory> factory);

		template<core::ComponentConcept... Cs>
		requires core::ValidConfigConcept<Cs...>
		bool Register(core::TypeList<Cs...>, std::string_view name);

		std::shared_ptr<core::Unit> Create(std::string_view name, core::World const*, core::UnitId, DataPack&& pack);

	private:
		using UnitFactory = std::shared_ptr<core::Unit>(*)(core::UnitFactory*, core::World const*, std::string_view, core::UnitId, DataPack&&);

		std::unique_ptr<core::UnitFactory> _commonFactory;
		std::unordered_map<std::string, UnitFactory> _factories;
	};
}

#include "UnitConfigRegistry.inl"
