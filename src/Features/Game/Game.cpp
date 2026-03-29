#include "Game.hpp"

#include "IO/Events/MapCreated.hpp"
#include "IO/Events/MarchEnded.hpp"
#include "IO/Events/MarchStarted.hpp"
#include "IO/Events/UnitAttacked.hpp"
#include "IO/Events/UnitDied.hpp"
#include "IO/Events/UnitMoved.hpp"
#include "IO/Events/UnitSpawned.hpp"

#include <cassert>

namespace sw
{
	using namespace core;

	Game::Game(std::unique_ptr<UnitConfigRegistry> configRegistry, std::shared_ptr<EventLog> logger, int maxTicks)
		: _configRegistry(std::move(configRegistry))
		, _logger(logger)
		, _world(nullptr)
		, _tick(0)
		, _maxTicks(maxTicks)
	{
	}

	void Game::CreateMap(uint32_t width, uint32_t height)
	{
		if (_world)
		{
			assert(false && "World already exists");
			return;
		}

		_world = std::make_unique<World>(width, height);
		_logger->log(_tick, io::MapCreated{width, height});
	}

	void Game::SpawnUnit(std::string_view name, UnitId id, DataPack&& pack)
	{
		auto unit = _configRegistry->Create(name, _world.get(), id, std::move(pack));
		_world->AddUnit(unit);
		_units.push_back(std::weak_ptr(unit));
		SetEventHandlers(unit);

		uint32_t x{};
		uint32_t y{};
		if (const auto* positionComponent = unit->GetComponent<PositionComponent>())
		{
			const auto& pos = positionComponent->GetData();
			x = pos.x;
			y = pos.y;
		}
		_logger->log(_tick, io::UnitSpawned{ unit->GetId(), std::string(unit->GetName()), x, y});
	}

	void Game::StartMarch(core::UnitId id, core::Position target)
	{
		assert(_world && "World is not assigned");

		auto unit = _world->GetUnit(id);

		if (!unit)
		{
			assert(false && "Can't find unit");
			return;
		}

		if (auto* movement = unit->GetComponent<MovementComponent>())
		{
			movement->StartMarch(target);
		}
	}

	void Game::Run()
	{
		bool canAct = true;
		while (_world->GetUnitAmount() > 1 && _tick < _maxTicks && canAct)
		{
			_tick++;
			_world->Step();

			canAct = false;
			for (std::weak_ptr<Unit> wUnit : _units)
			{
				if (!wUnit.expired())
				{
					auto unit = wUnit.lock();

					if (auto* actionController = unit->GetComponent<ActionControllerComponent>())
					{
						if (actionController->CanAct())
						{
							canAct = true;
							break;
						}
					}
				}
			}
		}
	}

	void Game::SetHealthLikeHandlers(std::shared_ptr<core::Unit> unit) noexcept
	{
		if (auto* health = unit->GetComponent<HealthComponent>())
		{
			auto deathHandle = health->onDeath.Subscribe([this](const Unit* unit)
				{
					_world->MarkUnitOnDeletion(unit->GetId());
					std::erase_if(_units, [unit](std::weak_ptr<Unit> item) { return item.expired() ? false : *item.lock() == *unit; });
					_logger->log(_tick, io::UnitDied{ unit->GetId() });
				});

			auto attackedHandle = health->onDamage.Subscribe(
				[this, cHealth = static_cast<HealthComponent const*>(health)]
				(int damage, const Unit* striker, const Unit* target)
				{
					_logger->log(_tick, io::UnitAttacked{ striker->GetId(), target->GetId(), static_cast<uint32_t>(damage),  static_cast<uint32_t>(cHealth->GetData().currentHealth) });
				});

			_eventHandles.push_back(deathHandle);
			_eventHandles.push_back(attackedHandle);
		}
	}

	void Game::SetMovementLikeHandlers(std::shared_ptr<core::Unit> unit) noexcept
	{
		if (auto* movement = unit->GetComponent<MovementComponent>())
		{
			auto moveHandle = movement->onMove.Subscribe([this](Position oldPos, Position newPos, const Unit* unit)
				{
					_logger->log(_tick, io::UnitMoved{ unit->GetId(), static_cast<uint32_t>(newPos.x), static_cast<uint32_t>(newPos.y) });
				});

			auto marchStartHandle = movement->onMarchStarted.Subscribe([this](core::Position target, const core::Unit* unit)
				{
					const auto* positionCom = unit->GetComponent<PositionComponent>();
					const auto& pos = positionCom->GetData();

					uint32_t x = pos.x;
					uint32_t y = pos.y;
					uint32_t targetX = target.x;
					uint32_t targetY = target.y;

					_logger->log(_tick, io::MarchStarted{ unit->GetId(), x, y, targetX, targetY });
				});

			auto marchEndedHandle = movement->onMarchFinished.Subscribe([this](core::Position target, const core::Unit* unit)
				{
					uint32_t x = target.x;
					uint32_t y = target.y;
					_logger->log(_tick, io::MarchEnded{ unit->GetId(), x, y });
				});

			_eventHandles.push_back(moveHandle);
			_eventHandles.push_back(marchStartHandle);
			_eventHandles.push_back(marchEndedHandle);
		}
	}

	void Game::SetEventHandlers(std::shared_ptr<Unit> unit) noexcept
	{
		SetHealthLikeHandlers(unit);
		SetMovementLikeHandlers(unit);
	}
}
