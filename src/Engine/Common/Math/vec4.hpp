#pragma once

#include "common.hpp"

namespace TBMath
{
	class Vec4
	{
	public:
		Vec4()
			: x(0.0f), y(0.0f), z(0.0f), w(0.0f)
		{}

		Vec4(Real x, Real y, Real z)
			: x(x), y(y), z(z), w(0.0f)
		{}

		Vec4(Real x, Real y, Real z, Real w)
			: x(x), y(y), z(z), w(w)
		{}

		Vec4 operator+(Vec4 const& v) const
		{
			return Vec4(
				x + v.x,
				y + v.y,
				z + v.z,
				w + v.w);
		}

		Vec4 operator+=(Vec4 const& v)
		{
			x += v.x;
			y += v.y;
			z += v.z;
			w += v.w;

			return *this;
		}

		Vec4 operator-(Vec4 const& v) const
		{
			return Vec4(
				x - v.x,
				y - v.y,
				z - v.z,
				w - v.w);
		}

		Vec4 operator-=(Vec4 const& v)
		{
			x -= v.x;
			y -= v.y;
			z -= v.z;
			w -= v.w;

			return *this;
		}

		Vec4 operator*(Vec4 const& v)
		{
			Real newW = w * v.w - x * v.x - y * v.y - z * v.z;
			Real newX = w * v.x + x * v.w + y * v.z - z * v.y;
			Real newY = w * v.y + y * v.w + z * v.x - x * v.z;
			Real newZ = w * v.z + z * v.w + x * v.y - y * v.x;

			return Vec4(newW, newX, newY, newZ);
		}

		Real x, y, z, w;
	};
}