#pragma once

#include "Core/Data/PositionData.hpp"

#include <array>

namespace sw::core
{
	std::array<Position, 8> GetAdjacentPositions(Position pos) noexcept;
}
