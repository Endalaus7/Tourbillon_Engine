#include "TransformComponent.h"
#include "Math/trans.hpp"
#include "ECSManager.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

const TBMath::Mat44& TourBillon::Transfrom::GetModelMatrix()
{
	TBMath::Mat44 model_mat = TBMath::modelMatrix(position, rotation, scale);
	//glm::mat4x4 model = glm::rotate(glm::mat4(1.0f), 0.f, glm::vec3(0.0f, 0.0f, 1.0f));

	return model_mat.transpose();
}

TBMath::Mat44& TourBillon::Transfrom::GetViewMatrix()
{
	TBMath::Vec3 lookat = position + getforward().normalizedCopy() * 3.464;
	TBMath::Vec3 up = getup();
	TBMath::Mat44 view_mat = TBMath::lookAt(position, lookat, up);
	return view_mat.transpose();
}

const TBMath::Mat44& TourBillon::Transfrom::GetRotateMatrix()
{
	TBMath::Mat44 rotateX = TBMath::rotateAroundAxis(TBMath::Vec3(1.f, 0.f, 0.f), TBMath::ToRadians(rotation.x));
	TBMath::Mat44 rotateY = TBMath::rotateAroundAxis(TBMath::Vec3(0.f, 1.f, 0.f), TBMath::ToRadians(rotation.y));
	TBMath::Mat44 rotateZ = TBMath::rotateAroundAxis(TBMath::Vec3(0.f, 0.f, 1.f), TBMath::ToRadians(rotation.z));

	glm::mat4 grotPitch = glm::rotate(glm::mat4(1.0f), rotation.x, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 grotYaw = glm::rotate(glm::mat4(1.0f), rotation.y, glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 grotRoll = glm::rotate(glm::mat4(1.0f), rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 total = grotRoll * grotPitch * grotRoll;

	TBMath::Mat44 rotationMatrix = rotateZ * rotateY * rotateX;

	return rotationMatrix;
}

TBMath::Vec3& TourBillon::Transfrom::getforward()
{
	TBMath::Mat44 rotMat = GetRotateMatrix();
	TBMath::Vec3 result = rotMat * UNIT_Z_VEC3;
	return result;
}

TBMath::Vec3& TourBillon::Transfrom::getright()
{
	TBMath::Mat44 rotMat = GetRotateMatrix();
	TBMath::Vec3 result = rotMat * UNIT_X_VEC3;
	return result;
}

TBMath::Vec3& TourBillon::Transfrom::getup()
{
	TBMath::Mat44 rotMat = GetRotateMatrix();
	TBMath::Vec3 result = rotMat * UNIT_Y_VEC3;
	return result;
}

void TourBillon::Transfrom::move(const TBMath::Vec3& offset)
{
	position += offset;
}

void TourBillon::Transfrom::rotate(const TBMath::Vec3& offset)
{
	rotation += offset;
}
