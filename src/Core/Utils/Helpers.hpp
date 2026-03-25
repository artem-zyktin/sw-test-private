#pragma once

#include <type_traits>

namespace sw::core
{
	template<typename T, typename... Ts>
	constexpr bool uniqueAmong = (!std::is_same_v<T, Ts> && ...);

	template<typename... Ts>
	constexpr bool allUnique = true;

	template<typename T, typename... Ts>
	requires (sizeof...(Ts) > 0)
	constexpr bool allUnique<T, Ts...> = uniqueAmong<T, Ts...>&& uniqueAmong<Ts...>;
}
