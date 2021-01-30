#include "MultipleRenderComponentSystem.h"

#include "core/system.h"

#include "core/coordinator.h"

#include <iostream>

extern Coordinator gCoordinator;


void MultipleRenderComponentSystem::Init(std::array <CustomCamera,4> *cameras)
{
	RenderSystem::Init(cameras);
}

void MultipleRenderComponentSystem::Update()
{
	
	for (auto const& entity : mEntities)
	{
		
		if(this->m_cameras_ptr)
		{			
			
			auto& render_mult_parts = gCoordinator.GetComponent<MultipleRenderComponent>(entity);
			
			if(render_mult_parts.render_parts_vec[0].texture_ptr)
			{
				for(size_t i = 0; i < render_mult_parts.render_parts_vec.size(); i++)
				{
					DrawTextureRec(*render_mult_parts.render_parts_vec[i].texture_ptr, 
									render_mult_parts.render_parts_vec[i].frame_rect, 
									render_mult_parts.render_parts_vec[i].position, 
									render_mult_parts.render_parts_vec[i].tint);
				}
			}	
			
			
			
		}
		
	}
}

