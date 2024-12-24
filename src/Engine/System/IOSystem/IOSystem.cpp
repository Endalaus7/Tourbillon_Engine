#include "IOSystem.h"
#include "fileIO/imageIO.h"
#include "keyIO/keyIO.h"
void TourBillon::IOSystem::initialize(SystemInitInfo* init_info)
{
	keyIO keyIOTool;
	keyIOTool.init();
	m_IOTools.push_back(&keyIOTool);
}
