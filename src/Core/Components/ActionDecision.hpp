#pragma once

namespace sw::core
{
	struct IActionComponent;

	struct ActionDecision
	{
		IActionComponent* component = nullptr;
		int priority = -1;

		bool IsValid() const noexcept;
	};
}