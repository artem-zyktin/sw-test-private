#pragma once

#include <concepts>

namespace sw::core
{
	struct IComponent
	{
		virtual ~IComponent() noexcept = default;
	};

	template<typename T>
	concept BasicComponentConcept = std::derived_from<T, IComponent>;
}
