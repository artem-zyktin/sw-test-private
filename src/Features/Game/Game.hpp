#pragma once

#include "Core.hpp"
#include "Features/Units.hpp"
#include "Features/Data.hpp"
#include "IO/System/EventLog.hpp"

#include <memory>
#include <string_view>
#include <vector>

namespace sw
{
	struct Game
	{
		Game(std::unique_ptr<UnitConfigRegistry> configRegistry, std::shared_ptr<EventLog> logger, int maxTicks = 99);

		void CreateMap(uint32_t width, uint32_t height);
		void SpawnUnit(std::string_view name, core::UnitId id, DataPack&& pack);
		void StartMarch(core::UnitId id, core::Position target);

		void Run();

	private:
		void SetHealthLikeHandlers(std::shared_ptr<core::Unit> unit) noexcept;
		void SetMovementLikeHandlers(std::shared_ptr<core::Unit> unit) noexcept;
		void SetEventHandlers(std::shared_ptr<core::Unit> unit) noexcept;

		std::unique_ptr<UnitConfigRegistry> _configRegistry;
		std::shared_ptr<EventLog> _logger;
		std::unique_ptr<core::World> _world;
		int _tick;
		int _maxTicks;
		std::vector<std::weak_ptr<core::Unit>> _units;
		std::vector<std::shared_ptr<core::EventHandle>> _eventHandles;
	};
}
