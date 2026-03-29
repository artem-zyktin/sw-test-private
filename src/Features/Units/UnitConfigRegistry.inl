#pragma once

namespace sw
{
	template<core::ComponentConcept ...Cs>
	requires core::ValidConfigConcept<Cs...>
	bool sw::UnitConfigRegistry::Register(core::TypeList<Cs...>, std::string_view name)
	{
		auto [it, result] = _factories.try_emplace(std::string(name), nullptr);

		if (!result)
		{
			return false;
		}

		it->second = +[](core::UnitFactory* factory, core::World const * world, std::string_view name_, core::UnitId id, DataPack&& data) -> std::shared_ptr<core::Unit>
		{
			return factory->CreateUnit(core::TypeList<Cs...>{}, world, id, name_, std::move(*(data.TryTake<typename Cs::DataType>()))...);
		};

		return true;
	}


}
