#include "CameraComponent.h"
#include "Math/trans.hpp"
#include "Math/quaternion.hpp"

#ifdef _DEBUG

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#endif // DEBUG


void TourBillon::Camera3D::move(const TBMath::Vec3& offset)
{
	pos += offset;
	lookat += offset;
}


void TourBillon::Camera3D::rotation(const Real& pitch, const Real& yaw, const Real& roll)
{
	TBMath::Mat44 rotateX = TBMath::rotateAroundAxis(TBMath::Vec3(1.f, 0.f, 0.f), pitch);
	TBMath::Mat44 rotateY = TBMath::rotateAroundAxis(TBMath::Vec3(0.f, 1.f, 0.f), yaw);
	TBMath::Mat44 rotateZ = TBMath::rotateAroundAxis(TBMath::Vec3(0.f, 0.f, 1.f), roll);
	TBMath::Mat44 rotationMatrix = rotateZ * rotateY * rotateX;

	TBMath::Vec3 dir = getDirection().normalizedCopy();
	TBMath::Vec3 newdir = rotationMatrix * dir;

	lookat = newdir.normalizedCopy() * (pos - lookat).length() + pos;
	//up = rotationMatrix * up;
}

TBMath::Vec3 TourBillon::Camera3D::getDirection()
{
	return lookat - pos;
}

TBMath::quaternion TourBillon::Camera3D::GetQuaternion()
{
	TBMath::quaternion quat;
	quat.setRotaionMartix(GetRotationMatrix());
	//glm::mat4 matlookat = glm::lookAt(glm::vec3(pos.x, pos.x, pos.x), glm::vec3(lookat.x, lookat.x, lookat.x), glm::vec3(up.x, up.x, up.x));
	//glm::mat4x4 view = glm::lookAt(glm::vec3(2.0f, 2.0f, -2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
	//glm::mat3 matrotate(view);
	//glm::quat q(matrotate);
	return quat;
}

TBMath::Mat33 TourBillon::Camera3D::GetRotationMatrix()
{
	TBMath::Mat33 result = TBMath::lookAtRotate(pos, lookat, up);
	TBMath::Mat44 view_mat = TBMath::lookAt(pos, lookat, up);
	return result;
}


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