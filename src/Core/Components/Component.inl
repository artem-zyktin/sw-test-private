#pragma once

#include "Component.hpp"

#include <utility>

namespace sw::core
{
	template<DataConcept T>
	Component<T>::Component(const World* world, const Unit* owner, DataType&& data) noexcept(std::is_nothrow_move_constructible_v<DataType>)
		: _world(world)
		, _owner(owner)
		, _data(std::move(data))
	{
	}

	template<DataConcept T>
	Unit const* Component<T>::GetOwner() const noexcept
	{
		return _owner;
	}

	template<DataConcept T>
	const Component<T>::DataType& Component<T>::GetData() const noexcept
	{
		return _data;
	}

	template<DataConcept T>
	Component<T>::DataType& Component<T>::GetData() noexcept
	{
		return _data;
	}

	template<DataConcept T>
	World const* Component<T>::GetWorld() const noexcept
	{
		return _world;
	}


}
