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
	for (auto const& entity : mEntities)
	{
		if(m_cameras_ptr)
		{
			//get render info component from entity
			auto& render_info = gCoordinator.GetComponent<RenderInfo>(entity);
					
			auto& transform = gCoordinator.GetComponent<Transform2D>(entity);
			
			if(render_info.texture_ptr)
			{
				if(transform.position.x && transform.position.y)
				{
					//render texture with frame rectangle clipping, position, and optional color tint
					DrawTextureRec(*render_info.texture_ptr, 
									render_info.frame_rect, 
									transform.position, 
									render_info.tint);
				}
				else
				{
					std::cout << "invalid transform!\n";
				}
				
			}
			else
			{
				
			}
		}
		
	}
}

