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
		
		if(this->m_cameras_ptr)
		{			
			
			auto& render_comp = gCoordinator.GetComponent<RenderComponent>(entity);
			auto& render_position = gCoordinator.GetComponent<RenderPosition>(entity);
			
			//if not multi part render
			if(!render_comp.multi_part)
			{
				if(render_comp.single_render_part.texture_ptr)
				{
					//adjust render component positions according to overall position
					Vector2 pos = {render_position.overall_position.x + render_comp.single_render_part.position.x,
								  render_position.overall_position.y + render_comp.single_render_part.position.y};
					
					
					DrawTextureRec(*render_comp.single_render_part.texture_ptr, 
									render_comp.single_render_part.frame_rect, 
									pos, 
									render_comp.single_render_part.tint);
				}
			}
			else
			{
				if(render_comp.multi_render_parts_vec[0].texture_ptr)
				{
					for(size_t i = render_comp.multi_render_parts_vec.size(); i >= 1; i--)
					{
						//render from last to first to make hair render over head
						
						//adjust render component positions according to overall position
						Vector2 pos = {render_position.overall_position.x + render_comp.multi_render_parts_vec[i-1].position.x,
									  render_position.overall_position.y + render_comp.multi_render_parts_vec[i-1].position.y};
												
						DrawTextureRec(*render_comp.multi_render_parts_vec[i-1].texture_ptr, 
										render_comp.multi_render_parts_vec[i-1].frame_rect, 
										pos, 
										render_comp.multi_render_parts_vec[i-1].tint);
					}
				}
			}
				
			
			
			
		}
		
	}
}

