#pragma once

#include "Core.hpp"

namespace sw
{
	struct ShootingAttackData : core::IData
	{
		int agility;
		int minRange, maxRange;

		ShootingAttackData(int agility_, int minRange_, int maxRange_) noexcept;
	};
}
