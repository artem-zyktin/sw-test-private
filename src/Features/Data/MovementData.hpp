#pragma once

#include "Core.hpp"

namespace sw
{
	struct MovementData : core::IData
	{
		int speed;
		
		MovementData(int speed_) noexcept;
	};
}