#include "ShootingAttackData.hpp"

sw::ShootingAttackData::ShootingAttackData(int agility_, int minRange_, int maxRange_) noexcept
	: IData()
	, agility(agility_)
	, minRange(minRange_)
	, maxRange(maxRange_)
{
}
