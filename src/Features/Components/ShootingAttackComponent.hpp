#pragma once

#include "Core.hpp"
#include "Features/Data.hpp"
#include "HealthComponent.hpp"

#include <vector>

namespace sw
{
	struct ShootingAttackComponent : core::Component<ShootingAttackData>, core::IActionComponent
	{
		ShootingAttackComponent(const core::World* world, const core::Unit* owner, ShootingAttackData&& data);

		core::Event<int/*damage points*/, const core::Unit*/*enemy*/, const core::Unit*/*self*/> onAttack;

		core::ActionDecision BuildAction() override;
		void ExecuteAction() override;

	private:
		const HealthComponent* _myHealth;
		const core::PositionComponent* _positionComponent;
		HealthComponent* _enemyHealth;

		std::vector<HealthComponent*> GetEnemyCandidates();
	};
}
