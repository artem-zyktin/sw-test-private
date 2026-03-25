#include "PositionData.hpp"

#include <algorithm>
#include <functional>

namespace sw::core
{
	Position::Position(int x_, int y_) noexcept
		: x(x_)
		, y(y_)
	{
	}

	Position::Position() noexcept
		: x(-1)
		, y(-1)
	{
	}

	int Position::Magnitude() const noexcept
	{
		return std::max(std::abs(x), std::abs(y));
	}

	Position Position::operator+(const Position& other) const noexcept
	{
		return { x + other.x, y + other.y };
	}

	Position Position::operator-(const Position& other) const noexcept
	{
		return { x - other.x, y - other.y };
	}

	bool Position::operator==(const Position& other) const noexcept
	{
		return x == other.x && y == other.y;
	}

	size_t Position::PositionHash::operator()(const Position& pos) const noexcept
	{
		auto hash = std::hash<int>();
		return (hash(pos.x)*1024 << 1) ^ (hash(pos.y)*512);
	}
}
