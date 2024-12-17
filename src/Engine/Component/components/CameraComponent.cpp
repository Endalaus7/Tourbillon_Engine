#include "CameraComponent.h"
#include "Math/trans.hpp"
TBMath::Mat44 TourBillon::Camera3D::GetVPMatrix()
{
	TBMath::Mat44 view_mat = TBMath::lookAt(pos, lookat, up);
	TBMath::Mat44 proj_mat = TBMath::perspective(ToRadians(fovX), fovX / fovY, nearClip, farClip);
	proj_mat[1][1] *= -1;

	return proj_mat * view_mat;
}