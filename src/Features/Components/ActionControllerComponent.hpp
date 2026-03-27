#pragma once

#include "Core.hpp"
#include "Features/Data.hpp"

#include <vector>

namespace sw
{
	struct ActionControllerComponent : core::Component<ActionControllerData>, core::IStepComponent
	{
		ActionControllerComponent(const core::World* world, const core::Unit* unit, ActionControllerData&& data);

		void PreStep() override;
		void Step() override;
		void PostStep() override {};

		void AddActionComponent(core::IActionComponent* component);

		bool CanAct() const noexcept;

	private:
		core::ActionDecision _decision;
		std::vector<core::IActionComponent*> _actionComponents;
	};
}
