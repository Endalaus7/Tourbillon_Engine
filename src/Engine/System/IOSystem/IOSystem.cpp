#include "IOSystem.h"

#include "keyIO/keyIO.h"
#include "fileIO/imageIO.h"

void TourBillon::IOSystem::initialize(SystemInitInfo* init_info)
{
	//由core决定选择哪些io工具

	keyIO* keyIOTool = new keyIO;
	keyIOTool->init();
	m_IOTools.push_back(keyIOTool);

	imageIO* imageIOTool = new imageIO;
	imageIOTool->init();
	m_IOTools.push_back(imageIOTool);
}
