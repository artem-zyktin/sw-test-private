#pragma once

#include "Core/Data/PositionData.hpp"
#include "Core/Unit/UnitId.hpp"

#include <cstdint>
#include <tuple>
#include <memory>
#include <array>
#include <unordered_map>
#include <vector>

namespace sw::core
{
	struct Unit;

	struct World
	{
		struct Cell
		{
			static constexpr uint32_t LAYER_AMOUNT = 4;

			Cell();

			bool Empty(size_t layer) const noexcept;

			UnitId operator[](size_t idx) const noexcept;
			UnitId& operator[](size_t idx) noexcept;

		private:
			std::array<UnitId, LAYER_AMOUNT> _data = {};
		};

		World(uint32_t width, uint32_t height);

		std::tuple<int, int> GetSize() const noexcept;
		Cell const * GetCell(Position pos) const noexcept;

		size_t GetUnitAmount() const noexcept;

		bool IsValidPosition(Position pos) const noexcept;

		void AddUnit(std::shared_ptr<Unit> unit);
		std::shared_ptr<const Unit>GetUnit(UnitId id) const noexcept;

		void MarkUnitOnDeletion(UnitId id) noexcept;

		bool MoveUnit(UnitId id, Position newPos) const noexcept;

		void Step();

	protected:
		Cell* GetCell(Position pos) noexcept;
		void RemoveMarkedUnits() noexcept;

	private:
		uint32_t _width, _height;
		std::unordered_map<UnitId, std::shared_ptr<Unit>> _units;
		std::vector<Unit*> _stepOrder;
		mutable std::unordered_map<Position, Cell, Position::PositionHash> _cells;

		std::vector<UnitId> _unitsOnDeletion;
	};
}
