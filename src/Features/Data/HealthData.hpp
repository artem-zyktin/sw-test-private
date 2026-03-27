#pragma once

#include "Core.hpp"

namespace sw
{
	struct HealthData : core::IData
	{
		int maxHealth, currentHealth;

		HealthData(int maxHaelth_, int currentHealth_) noexcept;
	};
}
