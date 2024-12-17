#pragma once
#include "common.hpp"

namespace TBMath
{
	class Vec2
	{
	public:
		Vec2()
			: x(0.0f), y(0.0f)
		{}

		Vec2(Real x, Real y)
			: x(x), y(y)
		{}

		Vec2 operator+(Vec2 const& rhs) const
		{
			return Vec2(
				x + rhs.x,
				y + rhs.y
			);
		}

		Vec2 operator+=(Vec2 const& rhs)
		{
			x += rhs.x;
			y += rhs.y;

			return *this;
		}

		Vec2 operator-(Vec2 const& rhs) const
		{
			return Vec2(
				x - rhs.x,
				y - rhs.y
			);
		}

		Vec2 operator-=(Vec2 const& rhs)
		{
			x -= rhs.x;
			y -= rhs.y;

			return *this;
		}

		Vec2 operator*(Vec2 const& rhs) const
		{
			return Vec2(
				x * rhs.x,
				y * rhs.y
			);
		}

		Vec2 operator*=(Vec2 const& rhs)
		{
			x *= rhs.x;
			y *= rhs.y;

			return *this;
		}

		Vec2 operator*(Real rhs) const
		{
			return Vec2(
				x * rhs,
				y * rhs
			);
		}

		Vec2 operator*=(Real rhs)
		{
			x *= rhs;
			y *= rhs;

			return *this;
		}


		Real x, y;
	};
}
