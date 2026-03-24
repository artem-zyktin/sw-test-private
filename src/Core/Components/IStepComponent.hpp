#pragma once

#include <concepts>

namespace sw::core
{
	struct IStepComponent
	{
		virtual void PreStep() = 0;
		virtual void Step() = 0;
		virtual void PostStep() = 0;

		virtual ~IStepComponent() noexcept = default;
	};

	template<typename T>
	concept StepComponentConcept = std::derived_from<T, IStepComponent>
	&& requires (T com)
	{
		{ com.PreStep() } -> std::same_as<void>;
		{ com.Step() } -> std::same_as<void>;
		{ com.PostStep() } -> std::same_as<void>;
	};
}
