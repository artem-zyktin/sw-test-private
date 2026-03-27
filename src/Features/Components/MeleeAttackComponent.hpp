#pragma once

#include "Core.hpp"
#include "Features/Data.hpp"

#include "HealthComponent.hpp"

#include <vector>

namespace sw
{
	struct MeleeAttackComponent : core::Component<MeleeAttackData>, core::IActionComponent
	{
		MeleeAttackComponent(const core::World* world, const core::Unit* owner, MeleeAttackData&& data);

		core::Event<int/*damage points*/, const sw::core::Unit*/*enemy*/, const sw::core::Unit*/*self*/> onAttack;

		core::ActionDecision BuildAction() override;
		void ExecuteAction() override;

	private:
		const HealthComponent* _myHealth;
		const core::PositionComponent* _positionComponent;
		HealthComponent* _enemyHealth;

		std::vector<HealthComponent*> GetEnemyCandidates();
	};
}
