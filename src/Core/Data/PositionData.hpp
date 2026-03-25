#pragma once

#include "IData.hpp"

#include <cstddef>

namespace sw::core
{
	struct Position : IData
	{
		int x, y;

		Position(int x_, int y_) noexcept;
		Position() noexcept;

		int Magnitude() const noexcept;

		Position operator+(const Position& other) const noexcept;
		Position operator-(const Position& other) const noexcept;

		bool operator==(const Position& other) const noexcept;

		struct PositionHash
		{
			size_t operator()(const Position& pos) const noexcept;
		};
	};
}
