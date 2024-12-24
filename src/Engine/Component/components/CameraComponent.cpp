#include "CameraComponent.h"
#include "Math/trans.hpp"
#include "Math/quaternion.hpp"

void TourBillon::Camera3D::move(const TBMath::Vec3& offset)
{
	pos += offset;
	lookat += offset;
}


void TourBillon::Camera3D::rotation(const Real& pitch, const Real& yaw, const Real& roll)
{
	TBMath::quaternion rotQuat;
	rotQuat.setRotaionMartix(pitch, yaw, roll);
	TBMath::quaternion newquat = rotQuat * GetQuaternion();
	lookat = newquat.getLookAt();
	up = newquat.getUp();
}

TBMath::Vec3 TourBillon::Camera3D::getDirection()
{
	return lookat - pos;
}

TBMath::quaternion TourBillon::Camera3D::GetQuaternion()
{
	TBMath::quaternion quat;
	quat.setRotaionMartix(GetRotationMatrix());
	return quat;
}

TBMath::Mat33 TourBillon::Camera3D::GetRotationMatrix()
{
	TBMath::Vec3 dir = getDirection();
	TBMath::Vec3 right = dir.cross(up);
	return TBMath::Mat33(dir, right, up);
}

//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
TBMath::Mat44 TourBillon::Camera3D::GetVPMatrix()
{
	TBMath::Mat44 view_mat = TBMath::lookAt(pos, lookat, up);
	TBMath::Mat44 proj_mat = TBMath::perspective(ToRadians(fovX), fovX / fovY, nearClip, farClip);
	proj_mat[1][1] *= -1;

	//glm::mat4x4 view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	//glm::mat4x4 proj = glm::perspective(glm::radians(60.0f), 1.f, 0.1f, 10.0f);
	//proj[1][1] *= -1;
	//glm::mat4x4 glmret = proj * view;
	TBMath::Mat44 tbret = proj_mat * view_mat;
	
	return tbret.transpose();
}