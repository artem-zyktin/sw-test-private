#include "ActionControllerComponent.hpp"

namespace sw
{
	ActionControllerComponent::ActionControllerComponent(const core::World* world, const core::Unit* unit, ActionControllerData&& data)
		: Component(world, unit, std::move(data))
		, IStepComponent()
		, _decision()
		, _actionComponents()
	{
		_actionComponents.reserve(16);
	}

	void ActionControllerComponent::PreStep()
	{
		for (auto* component : _actionComponents)
		{
			auto candidate = component->BuildAction();
			if (candidate.IsValid() && candidate.priority > _decision.priority)
			{
				_decision = candidate;
			}
		}
	}

	void ActionControllerComponent::Step()
	{
		if (_decision.IsValid())
		{
			_decision.component->ExecuteAction();
		}

		_decision = {};
	}

	void ActionControllerComponent::AddActionComponent(core::IActionComponent* component)
	{
		_actionComponents.push_back(component);
	}

	bool ActionControllerComponent::CanAct() const noexcept
	{
		for (auto* component : _actionComponents)
		{
			auto candidate = component->BuildAction();
			if (candidate.IsValid())
			{
				return true;
			}
		}

		return false;
	}
}
