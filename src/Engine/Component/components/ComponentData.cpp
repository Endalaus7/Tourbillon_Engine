#include "ComponentData.h"

float TourBillon::ToRadians(float degrees)
{
	return degrees * (M_PI / 180.0f);
}

float TourBillon::ToDegrees(float radians)
{
	return radians * (180.0f / M_PI);
}