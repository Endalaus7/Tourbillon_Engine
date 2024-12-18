#pragma once

#include "vec4.hpp"

namespace TBMath
{

	class Mat44
	{
	public:
		Mat44()
		{
			//@formatter:off
			m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
			m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = 0.0f;
			m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = 0.0f;
			m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
			//@formatter:on
		}
		Mat44(Real mat[4][4])
		{
			std::memcpy(m, mat, 16 * sizeof(mat[0][0]));
		}

		Mat44(Real mat00, Real mat01, Real mat02, Real mat03,
			  Real mat10, Real mat11, Real mat12, Real mat13, 
			  Real mat20, Real mat21, Real mat22, Real mat23,
			  Real mat30, Real mat31, Real mat32, Real mat33 )
		{
			m[0][0] = mat00; m[0][1] = mat01; m[0][2] = mat02; m[0][3] = mat03;
			m[1][0] = mat10; m[1][1] = mat11; m[1][2] = mat12; m[1][3] = mat13;
			m[2][0] = mat20; m[2][1] = mat21; m[2][2] = mat22; m[2][3] = mat23;
			m[3][0] = mat30; m[3][1] = mat31; m[3][2] = mat32; m[3][3] = mat33;
		}

		Mat44 operator*(Mat44 const& rhs)
		{
			Mat44 result;

			for (int row = 0; row < 4; ++row)
			{
				for (int col = 0; col < 4; ++col)
				{
					Real sum = 0.0f;

					for (int i = 0; i < 4; ++i)
					{
						sum += m[row][i] * rhs.m[i][col];
					}

					result.m[row][col] = sum;
				}
			}

			return result;
		}


		Vec4 operator*(Vec4 const& rhs)
		{
			return Vec4(
				(rhs.x * m[0][0]) + (rhs.y * m[0][1]) + (rhs.z * m[0][2]) + (rhs.w * m[0][3]),
				(rhs.x * m[1][0]) + (rhs.y * m[1][1]) + (rhs.z * m[1][2]) + (rhs.w * m[1][3]),
				(rhs.x * m[2][0]) + (rhs.y * m[2][1]) + (rhs.z * m[2][2]) + (rhs.w * m[2][3]),
				(rhs.x * m[3][0]) + (rhs.y * m[3][1]) + (rhs.z * m[3][2]) + (rhs.w * m[3][3]));
		}

		Real* operator[](int row) {
			return m[row]; // 返回该行的数组
		}

		Mat44 transpose()
		{
			Mat44 tmp = m;
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					m[i][j] = tmp[j][i];
				}
			}
			return *this;
		}

		Real m[4][4];
	};

}