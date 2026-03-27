#pragma once

#include "Core.hpp"

namespace sw
{
	struct MeleeAttackData : core::IData
	{
		int strength;

		MeleeAttackData(int strength_) noexcept;
	};
}
