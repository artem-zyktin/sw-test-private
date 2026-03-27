#include "MeleeAttackComponent.hpp"
#include "ActionControllerComponent.hpp"

#include <random>

namespace sw
{
	MeleeAttackComponent::MeleeAttackComponent(const core::World* world, const core::Unit* owner, MeleeAttackData&& data)
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


	core::ActionDecision MeleeAttackComponent::BuildAction()
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
		return { this, 100 };
	}

	void MeleeAttackComponent::ExecuteAction()
	{
		if (_enemyHealth)
		{
			const auto& data = GetData();
			const auto* owner = GetOwner();

			_enemyHealth->Hit(data.strength, owner);
			onAttack.Invoke(data.strength, _enemyHealth->GetOwner(), owner);
		}
	}

	std::vector<HealthComponent*> MeleeAttackComponent::GetEnemyCandidates()
	{
		assert(_positionComponent && "Melee attack component needs position component");

		const auto& myPosition = _positionComponent->GetData();
		auto adjacent = core::GetAdjacentPositions(myPosition);

		const auto* world = GetWorld();

		std::vector<HealthComponent*> candidates;
		for (auto& pos : adjacent)
		{
			if (!world->IsValidPosition(pos))
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

			auto unit = world->GetUnit((*cell)[0]);
			if (*unit == *GetOwner())
			{
				continue;
			}

			if (auto* healthComponent = unit->GetComponent<HealthComponent>())
			{
				candidates.push_back(healthComponent);
			}
		}

		return candidates;
	}


}
