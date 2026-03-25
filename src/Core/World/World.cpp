#include "World.hpp"
#include "Core/Unit/Unit.hpp"

#include <cassert>

namespace sw::core
{
	World::Cell::Cell()
		: _data()
	{
		_data.fill(INVALID_USER_ID);
	}

	bool World::Cell::Empty(size_t layer) const noexcept
	{
		assert((layer < LAYER_AMOUNT) && "Layer index is greater then layers conteined in cell");
		return _data[layer] == INVALID_USER_ID;
	}

	UnitId World::Cell::operator[](size_t idx) const noexcept
	{
		assert((idx < LAYER_AMOUNT) && "Layer index is greater then layers conteined in cell");
		return _data[idx];
	}
	UnitId& World::Cell::operator[](size_t idx) noexcept
	{
		assert((idx < LAYER_AMOUNT) && "Layer index is greater then layers conteined in cell");
		return _data[idx];
	}

	World::World(uint32_t width, uint32_t height)
		: _width(width)
		, _height(height)
		, _units()
		, _stepOrder()
		, _cells()
		, _unitsOnDeletion()
	{
		size_t maxCells = _width * _height;
		size_t maxUnitsAmount = maxCells * Cell::LAYER_AMOUNT;

		_units.reserve(maxUnitsAmount);
		_stepOrder.reserve(maxUnitsAmount);
		_cells.reserve(maxCells);
		_unitsOnDeletion.reserve(maxUnitsAmount);
	}

	std::tuple<int, int> World::GetSize() const noexcept
	{
		return { _width, _height };
	}

	World::Cell const * World::GetCell(Position pos) const noexcept
	{
		if (const auto it = _cells.find(pos); it != _cells.end())
		{
			return &(it->second);
		}

		return nullptr;
	}

	size_t World::GetUnitAmount() const noexcept
	{
		return _units.size();
	}

	World::Cell* World::GetCell(Position pos) noexcept
	{
		if (const auto it = _cells.find(pos); it != _cells.end())
		{
			return &(it->second);
		}

		return nullptr;
	}

	void World::RemoveMarkedUnits() noexcept
	{
		for (auto id : _unitsOnDeletion)
		{
			if (id == INVALID_USER_ID)
			{
				continue;
			}

			auto unit = GetUnit(id);
			if (unit == nullptr)
			{
				continue;
			}

			_units.erase(id);

			if (auto* posComponent = unit->GetComponent<PositionComponent>())
			{
				auto* cell = GetCell(posComponent->GetData());
				assert(cell && "Unit deletion error: can't find cell with existing unit");

				(*cell)[0] = INVALID_USER_ID;
			}

			std::erase(_stepOrder, unit.get());
		}

		_unitsOnDeletion.clear();
	}

	bool World::IsValidPosition(Position pos) const noexcept
	{
		return pos.x >= 0 && pos.x < _width && pos.y >= 0 && pos.y < _height;
	}

	void World::AddUnit(std::shared_ptr<Unit> unit)
	{
		if (auto* posComponent = unit->GetComponent<PositionComponent>())
		{
			auto position = posComponent->GetData();
			if (!IsValidPosition(position))
			{
				assert(false && "Invalid unit position");
				return;
			}

			auto id = unit->GetId();

			auto* cell = GetCell(position);
			if (!cell)
			{
				cell = &_cells[position];
			}

			if (!cell->Empty(0))
			{
				assert(false && "Can't place cell in the same position with an other unit");
				return;
			}

			(*cell)[0] = id;

			auto [it, result] = _units.try_emplace(id, nullptr);

			if (!result)
			{
				assert(false && "World already contains the same unit id");
				return;
			}

			it->second = unit;

			_stepOrder.push_back(unit.get());
		}
	}

	std::shared_ptr<const Unit> World::GetUnit(UnitId id) const noexcept
	{
		assert((id < INVALID_USER_ID) && "Invalid user id");

		if (auto it = _units.find(id); it != _units.end())
		{
			return it->second;
		}

		return nullptr;
	}

	void World::MarkUnitOnDeletion(UnitId id) noexcept
	{
		_unitsOnDeletion.push_back(id);
	}

	bool World::MoveUnit(UnitId id, Position newPos) const noexcept
	{
		if (id == INVALID_USER_ID)
		{
			return false;
		}

		auto unit = GetUnit(id);
		if (!unit)
		{
			return false;
		}

		if (!IsValidPosition(newPos))
		{
			return false;
		}

		auto* newCell = const_cast<World*>(this)->GetCell(newPos);
		if (newCell && !newCell->Empty(0))
		{
			return false;
		}
		else if (!newCell)
		{
			_cells[newPos] = Cell();
			newCell = &_cells[newPos];
		}

		assert(newCell && "Somehow new cell cannot be created");

		if (auto* posComponent = unit->GetComponent<PositionComponent>())
		{
			auto& oldPos = posComponent->GetData();

			auto* oldCell = const_cast<World*>(this)->GetCell(oldPos);
			assert(oldCell && "Somehow existing unit have no according occupied cell");

			(*oldCell)[0] = INVALID_USER_ID;
			(*newCell)[0] = id;

			oldPos = newPos;

			return true;
		}

		return false;
	}

	void World::Step()
	{
		for (auto unit : _stepOrder)
		{
			unit->Step();
		}

		RemoveMarkedUnits();
	}
}
