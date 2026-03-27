#pragma once

#include "Core.hpp"
#include "Features/Components.hpp"

namespace sw
{
	using SwordsmanConfiguration = core::TypeList<core::PositionComponent,
												  HealthComponent,
												  ActionControllerComponent,
												  MovementComponent,
												  MeleeAttackComponent>;
}
