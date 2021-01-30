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
			
			//general rendering 
			
			//get render info component from entity
			auto& render_info = gCoordinator.GetComponent<RenderInfo>(entity);
					
			
			if(render_info.texture_ptr)
			{
				
				//render texture with frame rectangle clipping, position, and optional color tint
				DrawTextureRec(*render_info.texture_ptr, 
								render_info.frame_rect, 
								render_info.position, 
								render_info.tint);
				
			}
			
			auto& render_body_parts = gCoordinator.GetComponent<RenderBodyParts>(entity);
			
			if(render_body_parts.body_parts[0].texture_ptr)
			{
				for(size_t i = 0; i < render_body_parts.body_parts.size(); i++)
				{
					DrawTextureRec(*render_body_parts.body_parts[i].texture_ptr, 
									render_body_parts.body_parts[i].frame_rect, 
									render_body_parts.body_parts[i].position, 
									render_body_parts.body_parts[i].tint);
				}
			}	
			
			
			
		}
		
	}
}

