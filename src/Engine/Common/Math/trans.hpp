#pragma once
#include "Mat44.hpp"
#include "Vec3.hpp"
#include "common.hpp"

namespace TBMath
{
    FORCE_INLINE Mat44 scaleMatrix(const Vec3& scale) {
        Mat44 scaleMat;
        scaleMat.m[0][0] = scale.x;
        scaleMat.m[1][1] = scale.y;
        scaleMat.m[2][2] = scale.z;
        return scaleMat;
    }
    FORCE_INLINE Mat44 rotateAroundAxis(Vec3& axis, float angle) {
        Vec3 u = axis.normalizedCopy(); // ȷ����ת���ǵ�λ����

        float cosA = cos(angle);
        float sinA = sin(angle);
        float oneMinusCosA = 1.0f - cosA;

        Mat44 rotationMat;

        // ������ת����
        rotationMat.m[0][0] = cosA + u.x * u.x * oneMinusCosA;
        rotationMat.m[0][1] = u.x * u.y * oneMinusCosA - u.z * sinA;
        rotationMat.m[0][2] = u.x * u.z * oneMinusCosA + u.y * sinA;
        rotationMat.m[0][3] = 0.0f;

        rotationMat.m[1][0] = u.y * u.x * oneMinusCosA + u.z * sinA;
        rotationMat.m[1][1] = cosA + u.y * u.y * oneMinusCosA;
        rotationMat.m[1][2] = u.y * u.z * oneMinusCosA - u.x * sinA;
        rotationMat.m[1][3] = 0.0f;

        rotationMat.m[2][0] = u.z * u.x * oneMinusCosA - u.y * sinA;
        rotationMat.m[2][1] = u.z * u.y * oneMinusCosA + u.x * sinA;
        rotationMat.m[2][2] = cosA + u.z * u.z * oneMinusCosA;
        rotationMat.m[2][3] = 0.0f;

        rotationMat.m[3][0] = 0.0f;
        rotationMat.m[3][1] = 0.0f;
        rotationMat.m[3][2] = 0.0f;
        rotationMat.m[3][3] = 1.0f;

        return rotationMat;
    }

    FORCE_INLINE Mat44 translateMatrix(const Vec3& translation) {
        Mat44 translateMat;
        translateMat.m[0][3] = translation.x;
        translateMat.m[1][3] = translation.y;
        translateMat.m[2][3] = translation.z;
        return translateMat;
    }

    FORCE_INLINE Mat44 modelMatrix(const Vec3& translation, const Vec3& rotation, const Vec3& scale) {
        Mat44 scaleMat = scaleMatrix(scale);
        Mat44 rotateXMat = rotateAroundAxis(Vec3(1.f, 0.f, 0.f),rotation.x);
        Mat44 rotateYMat = rotateAroundAxis(Vec3(0.f, 1.f, 0.f),rotation.y);
        Mat44 rotateZMat = rotateAroundAxis(Vec3(0.f, 0.f, 1.f),rotation.z);
        Mat44 translateMat = translateMatrix(translation);

        // ��ϱ任������ -> ��ת -> ƽ��
        Mat44 model = scaleMat * rotateXMat * rotateYMat * rotateZMat * translateMat;
        return model;
    }

    FORCE_INLINE Mat44 lookAt(const Vec3& eye, const Vec3& target, const Vec3& up)
    {
        Vec3 f = (target - eye).normalizedCopy();  // ǰ������
        Vec3 r = f.cross(up).normalizedCopy();     // ������
        Vec3 u = r.cross(f);                  // ������

        Mat44 result;

        // ������ת���󲿷�
        result.m[0][0] = r.x; result.m[0][1] = r.y; result.m[0][2] = r.z; result.m[0][3] = 0.0f;
        result.m[1][0] = u.x; result.m[1][1] = u.y; result.m[1][2] = u.z; result.m[1][3] = 0.0f;
        result.m[2][0] = -f.x; result.m[2][1] = -f.y; result.m[2][2] = -f.z; result.m[2][3] = 0.0f;

        // ����ƽ�ƾ��󲿷�
        result.m[3][0] = -r.dot(eye); result.m[3][1] = -u.dot(eye); result.m[3][2] = f.dot(eye); result.m[3][3] = 1.0f;

        return result;
    }

    //͸�Ӿ���
    //fovy:y����fov aspect:��߱� near:��ƽ�� far:Զƽ��
    FORCE_INLINE Mat44 perspective(float fovy, float aspect, float near, float far) 
    {
        Mat44 result;
        float tanHalfFovy = tan(fovy / 2.0f);

        // ����͸��ͶӰ����
        result.m[0][0] = 1.0f / (tanHalfFovy * aspect);
        result.m[0][1] = 0.0f;
        result.m[0][2] = 0.0f;
        result.m[0][3] = 0.0f;

        result.m[1][0] = 0.0f;
        result.m[1][1] = 1.0f / tanHalfFovy;
        result.m[1][2] = 0.0f;
        result.m[1][3] = 0.0f;

        result.m[2][0] = 0.0f;
        result.m[2][1] = 0.0f;
        result.m[2][2] = (far + near) / (near - far);
        result.m[2][3] = (2.0f * far * near) / (near - far);

        result.m[3][0] = 0.0f;
        result.m[3][1] = 0.0f;
        result.m[3][2] = -1.0f;
        result.m[3][3] = 0.0f;

        return result;
    }
}