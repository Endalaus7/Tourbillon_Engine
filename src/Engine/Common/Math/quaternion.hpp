#pragma once

#include "common.hpp"
#include "vec3.hpp"
#include "vec4.hpp"
#include "mat33.hpp"
namespace TBMath
{
	class quaternion
	{
	public:
        quaternion()
        {

        }
        quaternion(const Vec4& vec4)
            :quaternion(vec4.w,vec4.x,vec4.y,vec4.z)
        {

        }
        quaternion(Real w,Real x, Real y, Real z)
            :w(w),x(x),y(y),z(z)
        {
        }

        quaternion operator*(quaternion const& v)const
        {
            Real newW = w * v.w - x * v.x - y * v.y - z * v.z;
            Real newX = w * v.x + x * v.w + y * v.z - z * v.y;
            Real newY = w * v.y - y * v.w + z * v.x + x * v.z;
            Real newZ = w * v.z + z * v.w - x * v.y + y * v.x;

            return quaternion(newW, newX, newY, newZ);
        }

        Vec3 rotate(Vec3& v)//不确定对不对
        {
            quaternion v_q( v.x, v.y, v.z, 0.f);
            quaternion q_inv = conjugate();
            quaternion result_q = *this * v_q * q_inv;
            return Vec3(result_q.x, result_q.y, result_q.z);
        }
        quaternion conjugate() const {
            return quaternion(-x, -y, -z, w);
        }
        Vec3 getDir() { return rotate(Vec3(0, 0, -1)); }
        Vec3 getUp() { return rotate(Vec3(0, 1, 0)); }

        quaternion operator*(Vec4 const& v)
        {
            Real newW = w * v.w - x * v.x - y * v.y - z * v.z;
            Real newX = w * v.x + x * v.w + y * v.z - z * v.y;
            Real newY = w * v.y + y * v.w + z * v.x - x * v.z;
            Real newZ = w * v.z + z * v.w + x * v.y - y * v.x;

            return Vec4(newW, newX, newY, newZ);
        }
        quaternion angleAxis(Real radians, Vec3& axis)
        {
            Vec3 normalizedAxis = axis.normalizedCopy();
            float halfAngle = radians / 2.0f;
            float sinHalfAngle = std::sin(halfAngle);
            float cosHalfAngle = std::cos(halfAngle);

            w = cosHalfAngle;
            x = normalizedAxis.x * sinHalfAngle;
            y = normalizedAxis.y * sinHalfAngle;
            z = normalizedAxis.z * sinHalfAngle;
            return *this;
        }

        void setRotaionMartix(Real pitch, Real yaw, Real roll)
        {
            quaternion tmpQuat;
            quaternion pitchQuat = tmpQuat.angleAxis(pitch, Vec3(1.0f, 0.0f, 0.0f)); // Pitch绕X轴
            quaternion yawQuat = tmpQuat.angleAxis(yaw, Vec3(0.0f, 1.0f, 0.0f));   // Yaw绕Y轴
            quaternion rollQuat = tmpQuat.angleAxis(roll, Vec3(0.0f, 0.0f, 1.0f));  // Roll绕Z轴

            // 合并这些旋转
            *this = yawQuat * pitchQuat * rollQuat;
        }

		void setRotaionMartix(Mat33 mat)
		{
            const Real trace = mat[0][0] + mat[1][1] + mat[2][2];

            if (trace > 0.0f) {
                float s = 0.5f / sqrt(trace + 1.0f);
                w = 0.25f / s;
                x = (mat[2][1] - mat[1][2]) * s;
                y = (mat[0][2] - mat[2][0]) * s;
                z = (mat[1][0] - mat[0][1]) * s;
            }
            else {
                if (mat[0][0] > mat[1][1] && mat[0][0] > mat[2][2]) {
                    float s = 2.0f * sqrt(1.0f + mat[0][0] - mat[1][1] - mat[2][2]);
                    w = (mat[2][1] - mat[1][2]) / s;
                    x = 0.25f * s;
                    y = (mat[0][1] + mat[1][0]) / s;
                    z = (mat[0][2] + mat[2][0]) / s;
                }
                else if (mat[1][1] > mat[2][2]) {
                    float s = 2.0f * sqrt(1.0f + mat[1][1] - mat[0][0] - mat[2][2]);
                    w = (mat[0][2] - mat[2][0]) / s;
                    x = (mat[0][1] + mat[1][0]) / s;
                    y = 0.25f * s;
                    z = (mat[1][2] + mat[2][1]) / s;
                }
                else {
                    float s = 2.0f * sqrt(1.0f + mat[2][2] - mat[0][0] - mat[1][1]);
                    w = (mat[1][0] - mat[0][1]) / s;
                    x = (mat[0][2] + mat[2][0]) / s;
                    y = (mat[1][2] + mat[2][1]) / s;
                    z = 0.25f * s;
                }
            }
		}
        Real w, x, y, z;
	};
}