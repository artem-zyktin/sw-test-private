#include "PositionComponent.hpp"

namespace sw::core
{
	void PositionComponent::Move(Position newPos)
	{
		GetData() = newPos;
	}
}
