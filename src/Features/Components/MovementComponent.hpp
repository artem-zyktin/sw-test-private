#pragma once

#include "Core.hpp"
#include "Features/Data.hpp"
#include "Features/Components.hpp"

namespace sw
{
	struct MovementComponent : core::Component<MovementData>, core::IActionComponent
	{
		MovementComponent(const core::World* world, const core::Unit* owner, MovementData&& data);

		core::Event<core::Position/*target*/, const core::Unit*/*self*/> onMarchStarted;
		core::Event<core::Position/*target*/, const core::Unit*/*self*/> onMarchFinished;
		core::Event<core::Position/*old pos*/, core::Position/*new pos*/, const core::Unit*/*self*/> onMove;


		core::ActionDecision BuildAction() override;
		void ExecuteAction() override;

		void StartMarch(core::Position pos);

	private:
		static const core::Position _invalidPosition;

		const HealthComponent* _myHealth;
		const core::PositionComponent* _positionComponent;
		core::Position _targetPos;
		core::Position _nextStep;

		core::Position GetBestNextStepCandidate();
	};
}
