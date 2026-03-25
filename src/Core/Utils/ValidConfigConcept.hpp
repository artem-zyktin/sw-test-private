#pragma once

#include "Helpers.hpp"
#include "Core/Components/Component.hpp"

namespace sw::core
{
	template<typename... Ts>
	concept ValidConfigConcept = (ComponentConcept<Ts> && ...)
	&& allUnique<Ts...>
	&& allUnique<typename Ts::DataType...>;
}
