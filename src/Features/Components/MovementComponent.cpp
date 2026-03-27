#include "MovementComponent.hpp"
#include "ActionControllerComponent.hpp"

#include <cassert>

namespace sw
{
	const core::Position MovementComponent::_invalidPosition = core::Position{ -1, -1 };

	MovementComponent::MovementComponent(const core::World* world, const core::Unit* owner, MovementData&& data)
		: Component(world, owner, std::move(data))
		, IActionComponent()
		, onMarchStarted()
		, onMarchFinished()
		, onMove()
		, _myHealth(owner->GetComponent<HealthComponent>())
		, _positionComponent(owner->GetComponent<core::PositionComponent>())
		, _targetPos(_invalidPosition)
		, _nextStep(_invalidPosition)
	{
		if (auto* actionController = owner->GetComponent<ActionControllerComponent>())
		{
			actionController->AddActionComponent(this);
		}
	}

	core::ActionDecision MovementComponent::BuildAction()
	{
		if (!_myHealth->IsAlive())
		{
			return {};
		}

		if (_targetPos == _invalidPosition)
		{
			return {};
		}

		auto candidate = GetBestNextStepCandidate();

		if (candidate == _invalidPosition)
		{
			return {};
		}

		_nextStep = candidate;

		return { this, 10 };
	}

	void MovementComponent::ExecuteAction()
	{
		if (_nextStep == _invalidPosition)
		{
			return;
		}

		auto data = _positionComponent->GetData();
		if (GetWorld()->MoveUnit(GetOwner()->GetId(), _nextStep))
		{
			const auto* owner = GetOwner();

			onMove.Invoke(data, _nextStep, owner);

			if (_nextStep == _targetPos)
			{
				onMarchFinished.Invoke(_targetPos, owner);
				_targetPos = _invalidPosition;
			}

			_nextStep = _invalidPosition;
		}
	}

	void MovementComponent::StartMarch(core::Position pos)
	{
		_targetPos = pos;
		onMarchStarted.Invoke(_targetPos, GetOwner());
	}

	core::Position MovementComponent::GetBestNextStepCandidate()
	{
		assert(_positionComponent && "Movement component needs position component");

		const auto& myPosition = _positionComponent->GetData();
		auto adjacents = core::GetAdjacentPositions(myPosition);

		const auto* world = GetWorld();

		auto distance = (_targetPos - myPosition).Magnitude();

		core::Position bestCandidate = _invalidPosition;

		for (const auto& pos : adjacents)
		{
			if (!world->IsValidPosition(pos))
			{
				continue;
			}

			const auto* cell = world->GetCell(pos);
			if (cell && !cell->Empty(0))
			{
				continue;
			}

			if (pos == _targetPos)
			{
				bestCandidate = pos;
			}

			auto newDistance = (_targetPos - pos).Magnitude();
			if (newDistance < distance)
			{
				distance = newDistance;
				bestCandidate = pos;
			}
		}

		return bestCandidate;
	}


}
