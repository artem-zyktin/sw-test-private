#include "ShootingAttackComponent.hpp"
#include "ActionControllerComponent.hpp"

#include <random>

namespace sw
{
	ShootingAttackComponent::ShootingAttackComponent(const core::World* world, const core::Unit* owner, ShootingAttackData&& data)
		: Component(world, owner, std::move(data))
		, IActionComponent()
		, onAttack()
		, _myHealth(owner->GetComponent<HealthComponent>())
		, _positionComponent(owner->GetComponent<core::PositionComponent>())
		, _enemyHealth(nullptr)
	{
		if (auto* actionController = owner->GetComponent<ActionControllerComponent>())
		{
			actionController->AddActionComponent(this);
		}
	}

	core::ActionDecision ShootingAttackComponent::BuildAction()
	{
		if (!_myHealth->IsAlive())
		{
			return {};
		}

		auto candidates = GetEnemyCandidates();

		if (candidates.size() == 0)
		{
			return {};
		}

		_enemyHealth = candidates[rand() % candidates.size()];
		return { this, 50 };
	}

	void ShootingAttackComponent::ExecuteAction()
	{
		if (_enemyHealth)
		{
			const auto& data = GetData();
			const auto* owner = GetOwner();

			_enemyHealth->Hit(data.agility, owner);
			onAttack.Invoke(data.agility, _enemyHealth->GetOwner(), owner);
		}
	}

	std::vector<HealthComponent*> ShootingAttackComponent::GetEnemyCandidates()
	{
		assert(_positionComponent && "Shooting attack component needs position component");

		const auto& myPosition = _positionComponent->GetData();
		const auto* world = GetWorld();
		const auto& data = GetData();

		auto [worldWidth, worldHeight] = world->GetSize();

		std::vector<HealthComponent*> candidates;
		for (int i = 0; i < worldWidth; ++i)
		{
			for (int j = 0; j < worldHeight; ++j)
			{
				auto pos = core::Position{ i, j };

				if (!world->IsValidPosition(pos))
				{
					continue;
				}

				auto distance = (pos - myPosition).Magnitude();
				if (distance <= data.minRange || distance > data.maxRange)
				{
					continue;
				}

				const auto* cell = world->GetCell(pos);
				if (!cell)
				{
					continue;
				}

				if (cell->Empty(0))
				{
					continue;
				}

				if (auto* healthComponent = world->GetUnit((*cell)[0])->GetComponent<HealthComponent>())
				{
					candidates.push_back(healthComponent);
				}
			}
		}

		return candidates;
	}


}
