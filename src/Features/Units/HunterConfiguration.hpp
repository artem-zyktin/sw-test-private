#pragma once

#include "Core.hpp"
#include "Features/Components.hpp"

namespace sw
{
	using HunterConfiguration = core::TypeList<core::PositionComponent,
											  HealthComponent,
											  ActionControllerComponent,
											  MovementComponent,
											  MeleeAttackComponent,
											  ShootingAttackComponent>;
}
