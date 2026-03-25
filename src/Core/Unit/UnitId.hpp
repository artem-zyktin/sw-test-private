#pragma once

#include <cstdint>
#include <limits>

namespace sw::core
{
	using UnitId = uint32_t;
	inline constexpr UnitId INVALID_USER_ID = std::numeric_limits<UnitId>::max();
}
