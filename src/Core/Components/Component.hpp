#pragma once

#include "IComponent.hpp"
#include "Core/Data/IData.hpp"

#include <concepts>

namespace sw::core
{
	struct Unit;
	struct World;

	template<DataConcept T>
	struct Component : IComponent
	{
		using DataType = T;

		Component(const World* world, const Unit* owner, DataType&& data) noexcept (std::is_nothrow_move_constructible_v<DataType>);

		// strong concept - Unit and Data can't be modified outside of Component
		World const* GetWorld() const noexcept;
		Unit const* GetOwner() const noexcept;
		const DataType& GetData() const noexcept;

	protected:
		DataType& GetData() noexcept;

	private:
		World const* _world;
		Unit const* _owner;
		DataType _data;
	};

	template<typename T>
	concept ComponentConcept = BasicComponentConcept<T>
	&& requires (const T com)
	{
		typename T::DataType;
		{ com.GetWorld() } -> std::same_as<World const*>;
		{ com.GetOwner() } -> std::same_as<Unit const*>;
		{ com.GetData() } -> std::same_as<const typename T::DataType&>;
	}
	&& std::derived_from<T, Component<typename T::DataType>>;
}

#include "Component.inl"
