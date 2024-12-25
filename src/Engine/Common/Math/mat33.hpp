#pragma once

#include "vec3.hpp"

namespace TBMath
{

	class Mat33
	{
	public:
		Mat33()
		{
			m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; 
			m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; 
			m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f;
		}
		Mat33(Real mat[3][3])
		{
			std::memcpy(m, mat, 9 * sizeof(mat[0][0]));
		}

		Mat33(Real mat00, Real mat01, Real mat02, 
			  Real mat10, Real mat11, Real mat12,  
			  Real mat20, Real mat21, Real mat22
			  )
		{
			m[0][0] = mat00; m[0][1] = mat01; m[0][2] = mat02;
			m[1][0] = mat10; m[1][1] = mat11; m[1][2] = mat12;
			m[2][0] = mat20; m[2][1] = mat21; m[2][2] = mat22;
		}

		Mat33(Vec3 row1, Vec3 row2, Vec3 row3)
		{
			m[0][0] = row1.x; m[0][1] = row1.y; m[0][2] = row1.z;
			m[1][0] = row2.x; m[1][1] = row2.y; m[1][2] = row2.z;
			m[2][0] = row3.x; m[2][1] = row3.y; m[2][2] = row3.z;
		}

		Mat33 operator*(Mat33 const& rhs)
		{
			Mat33 result;

			for (int row = 0; row < 3; ++row)
			{
				for (int col = 0; col < 3; ++col)
				{
					Real sum = 0.0f;

					for (int i = 0; i < 3; ++i)
					{
						sum += m[row][i] * rhs.m[i][col];
					}

					result.m[row][col] = sum;
				}
			}

			return result;
		}


		Vec3 operator*(Vec3 const& rhs)
		{
			return Vec3(
				(rhs.x * m[0][0]) + (rhs.y * m[0][1]) + (rhs.z * m[0][2]),
				(rhs.x * m[1][0]) + (rhs.y * m[1][1]) + (rhs.z * m[1][2]),
				(rhs.x * m[2][0]) + (rhs.y * m[2][1]) + (rhs.z * m[2][2])
				);
		}

		Real* operator[](int row) {
			return m[row]; // 返回该行的数组
		}

		Mat33 transpose()
		{
			Mat33 tmp = m;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					m[i][j] = tmp[j][i];
				}
			}
			return *this;
		}

		Real m[3][3];
	};

}