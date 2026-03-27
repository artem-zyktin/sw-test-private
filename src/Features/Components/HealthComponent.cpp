#include "HealthComponent.hpp"

#include <algorithm>

namespace sw
{
	using namespace core;

	HealthComponent::HealthComponent(const core::World* world, const core::Unit* owner, HealthData&& data)
		: Component(world, owner, std::move(data))
		, IStepComponent()
		, _isAlive(GetData().currentHealth > 0)
	{
	}
	int HealthComponent::Hit(int points, const sw::core::Unit* source) noexcept
	{
		onHit.Invoke(source, GetOwner());

		auto& data = GetData();

		int actual_points = std::clamp(points, 0, data.currentHealth);
		if (actual_points == 0)
		{
			return actual_points;
		}

		data.currentHealth -= actual_points;
		onDamage.Invoke(actual_points, source, GetOwner());

		return actual_points;
	}

	int HealthComponent::Heal(int points, const sw::core::Unit* source) noexcept
	{
		onHealAttempt.Invoke(source, GetOwner());

		auto& data = GetData();

		int actual_points = std::clamp(points, 0, data.maxHealth - data.currentHealth);
		if (actual_points == 0)
		{
			return actual_points;
		}

		data.currentHealth += actual_points;
		onHeal.Invoke(actual_points, source, GetOwner());

		return actual_points;
	}

	bool HealthComponent::IsAlive() const noexcept
	{
		return _isAlive;
	}

	void HealthComponent::PostStep()
	{
		if (GetData().currentHealth <= 0 && _isAlive)
		{
			onDeath.Invoke(GetOwner());
			_isAlive = false;
		}
	}
}
