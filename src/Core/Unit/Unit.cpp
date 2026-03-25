#include "Unit.hpp"

namespace sw::core
{
	Unit::ComponentId Unit::_nextComponentId = 0;

	UnitId Unit::GetId() const noexcept
	{
		return _id;
	}

	std::string_view Unit::GetName() const noexcept
	{
		return _name;
	}

	void Unit::Step()
	{
		for (auto component : _stepComponents)
		{
			component->PreStep();
		}

		for (auto component : _stepComponents)
		{
			component->Step();
		}

		for (auto component : _stepComponents)
		{
			component->PostStep();
		}
	}

	bool Unit::operator==(const Unit& other) const noexcept
	{
		return _id == other._id;
	}

	Unit::Unit(UnitId id, std::string_view name)
		: _id(id)
		, _name(name)
		, _components()
		, _stepComponents()
	{
		_stepComponents.reserve(16);
	}

}
