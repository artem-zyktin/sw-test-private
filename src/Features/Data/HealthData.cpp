#include "HealthData.hpp"

sw::HealthData::HealthData(int maxHealth_, int currentHealth_) noexcept
	: IData()
	, maxHealth(maxHealth_)
	, currentHealth(currentHealth_)
{
}
