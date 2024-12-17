#include "CameraComponent.h"
#include "Math/trans.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
TBMath::Mat44 TourBillon::Camera3D::GetVPMatrix()
{
	TBMath::Mat44 view_mat = TBMath::lookAt(pos, lookat, up);
	TBMath::Mat44 proj_mat = TBMath::perspective(ToRadians(fovX), fovX / fovY, nearClip, farClip);
	proj_mat[1][1] *= -1;

	//view_mat.transpose();
	//proj_mat.transpose();
	//
	//glm::mat4x4 view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	//glm::mat4x4 proj = glm::perspective(glm::radians(60.0f), 1.f, 0.1f, 10.0f);
	//proj[1][1] *= -1;
	//
	//glm::mat4x4 glmret = proj * view;
	//TBMath::Mat44 tbret = proj_mat * view_mat;

	
	return proj_mat * view_mat;
}