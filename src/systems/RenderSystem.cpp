#include "RenderSystem.h"

#include "core/system.h"

#include "core/coordinator.h"

#include <iostream>

extern Coordinator gCoordinator;


void RenderSystem::Init(std::array <CustomCamera,4> *cameras)
{
	m_cameras_ptr = cameras;
}

void RenderSystem::Update()
{
	
	
}

