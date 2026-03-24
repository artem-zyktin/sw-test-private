#pragma once

#include <concepts>

namespace sw::core
{
	struct IData
	{
		virtual ~IData() noexcept = default;
	};

	template<typename T>
	concept DataConcept = std::derived_from<T, IData>;
}
