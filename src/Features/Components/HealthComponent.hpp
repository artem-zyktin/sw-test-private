#pragma once

#include "Core.hpp"
#include "Features/Data.hpp"

namespace sw
{
	struct HealthComponent : core::Component<HealthData>, core::IStepComponent
	{
		HealthComponent(const core::World* world, const core::Unit* owner, HealthData&& data);

		core::Event<const sw::core::Unit*/*source*/, const sw::core::Unit*/*self*/> onHit{};
		core::Event<int /*actual damage points*/, const sw::core::Unit*/*source*/, const core::Unit*/*self*/> onDamage{};
		core::Event<const sw::core::Unit*/*source*/, const sw::core::Unit*/*self*/> onHealAttempt{};
		core::Event<int/*actual heal points*/, const sw::core::Unit*/*source*/, const core::Unit*/*self*/> onHeal{};
		core::Event<const sw::core::Unit* /*self*/> onDeath;

		int Hit(int points, const sw::core::Unit* source) noexcept;
		int Heal(int points, const sw::core::Unit* source) noexcept;

		bool IsAlive() const noexcept;

		void PreStep() override {};
		void Step() override {};
		void PostStep() override;

	private:
		bool _isAlive;
	};
}
