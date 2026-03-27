#include "PositionUtils.hpp"

namespace sw::core
{
	std::array<Position, 8> GetAdjacentPositions(Position pos) noexcept
	{
		return { pos + Position{0, 1}, pos + Position{1, 1}, pos + Position{1, 0}, pos + Position{1, -1},
				 pos + Position{0, -1}, pos + Position{-1, -1}, pos + Position{-1, 0}, pos + Position{-1, 1} };
	}
}
