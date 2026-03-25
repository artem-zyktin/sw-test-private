#include "ActionDecision.hpp"

namespace sw::core
{
	bool ActionDecision::IsValid() const noexcept
	{
		return priority > 0 && component != nullptr;
	}
}
