#pragma once
#include "common.hpp"

namespace TBMath
{
	class Vec3
	{
	public:
		Vec3()
			: x(0.0f), y(0.0f), z(0.0f)
		{}

		Vec3(Real x, Real y, Real z)
			: x(x), y(y), z(z)
		{}

		Vec3 operator+(Vec3 const& rhs) const
		{
			return Vec3(
				x + rhs.x,
				y + rhs.y,
				z + rhs.z);
		}

		Vec3 operator+=(Vec3 const& rhs)
		{
			x += rhs.x;
			y += rhs.y;
			z += rhs.z;

			return *this;
		}

		Vec3 operator-() const
		{
			return Vec3(
				-x,
				-y,
				-z);
		}

		Vec3 operator-(Vec3 const& rhs) const
		{
			return Vec3(
				x - rhs.x,
				y - rhs.y,
				z - rhs.z);
		}

		Vec3 operator-=(Vec3 const& rhs)
		{
			x -= rhs.x;
			y -= rhs.y;
			z -= rhs.z;

			return *this;
		}

		Vec3 operator*(Vec3 const& rhs) const
		{
			return Vec3(
				x * rhs.x,
				y * rhs.y,
				z * rhs.z);
		}

		Vec3 operator*=(Vec3 const& rhs)
		{
			x *= rhs.x;
			y *= rhs.y;
			z *= rhs.z;

			return *this;
		}

		Vec3 operator*(Real rhs) const
		{
			return Vec3(
				x * rhs,
				y * rhs,
				z * rhs);
		}

		Vec3 operator*=(Real rhs)
		{
			x *= rhs;
			y *= rhs;
			z *= rhs;

			return *this;
		}
		Vec3 operator/(Real rhs) const
		{
			return Vec3(
				x / rhs,
				y / rhs,
				z / rhs);
		}
		Vec3 operator/=(Real rhs)
		{
			x /= rhs;
			y /= rhs;
			z /= rhs;

			return *this;
		}

		void normalized()
		{
			float len = sqrt(x * x + y * y + z * z);
			*this /= len;
		}

		Vec3 normalizedCopy()
		{
			float len = sqrt(x * x + y * y + z * z);
			return Vec3(x / len, y / len, z / len);
		}

		// 向量点积
		float dot(const Vec3& other) const {
			return x * other.x + y * other.y + z * other.z;
		}

		// 向量叉积
		Vec3 cross(const Vec3& other) const {
			return Vec3(
				y * other.z - z * other.y,
				z * other.x - x * other.z,
				x * other.y - y * other.x
			);
		}
		Real length()const
		{
			return sqrt(x * x + y * y + z * z);
		}

		Real x, y, z;
	};
}
