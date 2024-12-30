#include "CameraComponent.h"
#include "Math/trans.hpp"
#include "Math/quaternion.hpp"
//#include "TransformComponent.h"
#ifdef _DEBUG
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#endif // DEBUG


TBMath::Mat44 TourBillon::Camera3D::GetProjMatrix()
{
	TBMath::Mat44 proj_mat = TBMath::perspective(TBMath::ToRadians(fovX), fovX / fovY, nearClip, farClip);
	proj_mat[1][1] *= -1;
	return proj_mat.transpose();
}
