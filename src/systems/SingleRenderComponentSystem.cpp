#include "SingleRenderComponentSystem.h"

#include "core/system.h"

#include "core/coordinator.h"

#include <iostream>

extern Coordinator gCoordinator;


void SingleRenderComponentSystem::Init(std::array <CustomCamera,4> *cameras)
{
	RenderSystem::Init(cameras);
}

void SingleRenderComponentSystem::Update()
{
	
	for (auto const& entity : mEntities)
	{
		
		if(this->m_cameras_ptr)
		{			
			//general rendering 
			
			//get render info component from entity
			auto& render_info = gCoordinator.GetComponent<SingleRenderComponent>(entity);
					
			
			if(render_info.render_part.texture_ptr)
			{
				
				//render texture with frame rectangle clipping, position, and optional color tint
				DrawTextureRec(*render_info.render_part.texture_ptr, 
								render_info.render_part.frame_rect, 
								render_info.render_part.position, 
								render_info.render_part.tint);
				
			}
			
			
		}
		
	}
}

