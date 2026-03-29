#include "UnitConfigRegistry.hpp"

namespace sw
{
	UnitConfigRegistry::UnitConfigRegistry(std::unique_ptr<core::UnitFactory> factory)
		: _commonFactory(std::move(factory))
		, _factories()
	{
	}

	std::shared_ptr<core::Unit> UnitConfigRegistry::Create(std::string_view name, const core::World* world, core::UnitId id, DataPack&& pack)
	{
		if (auto it = _factories.find(std::string(name)); it != _factories.end())
		{
			return it->second(_commonFactory.get(), world, name, id, std::move(pack));
		}

		return nullptr;
	}
}
