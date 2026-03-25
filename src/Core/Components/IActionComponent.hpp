#pragma once

#include "ActionDecision.hpp"

namespace sw::core
{
	struct IActionComponent
	{
		virtual ActionDecision BuildAction() = 0;
		virtual void ExecuteAction() = 0;

		virtual ~IActionComponent() noexcept = default;
	};

}
