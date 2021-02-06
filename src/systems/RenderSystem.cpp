#include "RenderSystem.h"

#include "core/system.h"

#include "core/coordinator.h"

#include <iostream>

extern Coordinator gCoordinator;


void RenderSystem::Init(std::array <CustomCamera,4> *cameras)
{
	m_cameras_ptr = cameras;
}

bool IsObjectInCameraView(float& posX, float& posY, Rectangle& camera_rect)
{
	
	if(posX < camera_rect.x)
	{
		return false;
	}
	else if(posX > camera_rect.x + (camera_rect.width / 2))
	{
		return false;
	}
	
	if(posY < camera_rect.y)
	{
		return false;
	}
	else if(posY > camera_rect.y + (camera_rect.height / 2))
	{
		return false;
	}
	
	return true;
}

void RenderSystem::Update()
{
	for (auto const& entity : mEntities)
	{
		auto& render_comp = gCoordinator.GetComponent<RenderComponent>(entity);
		auto& transform = gCoordinator.GetComponent<Transform2D>(entity);
		
		if(this->m_cameras_ptr)
		{			
			
			//for every camera
			for(size_t i = 0; i < m_cameras_ptr->size(); i++)
			{
				bool renderObjectBool = IsObjectInCameraView(transform.position.x,transform.position.y,m_cameras_ptr->at(i).camera_rect);
				
				//if renderable object is within camera bounds.
				if(renderObjectBool)
				{
					//change render positionof renderable object relative to camera
					auto& render_position = gCoordinator.GetComponent<RenderPosition>(entity);
						
					render_position.overall_position.x = transform.position.x - m_cameras_ptr->at(i).camera_rect.x;
					render_position.overall_position.y = transform.position.y - m_cameras_ptr->at(i).camera_rect.y;
					
					//render object
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
		
	}
}

