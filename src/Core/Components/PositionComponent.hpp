#pragma once

#include "Component.hpp"
#include "Core/Data/PositionData.hpp"

namespace sw::core
{
	struct PositionComponent : Component<Position>
	{
		using Component<Position>::Component;

	protected:
		friend struct World;

		void Move(Position newPos);
	};
}
