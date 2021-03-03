#include "AnimationSystem.h"

#include "core/system.h"

#include "core/coordinator.h"

#include <iostream>

extern Coordinator gCoordinator;


void AnimationSystem::Init()
{
	m_time_counter = 0;
}

//face direction order: SOUTH=0,SOUTHWEST,WEST,NORTHWEST,NORTH,NORTHEAST,EAST,SOUTHEAST

void AnimationSystem::Update(float& dt)
{
	
	for (auto const& entity : mEntities)
	{
			
		auto& transform = gCoordinator.GetComponent<Transform2D>(entity);
		auto& anim_comp = gCoordinator.GetComponent<Animation>(entity);
		auto& render_comp = gCoordinator.GetComponent<RenderComponent>(entity);
		
		switch(anim_comp.info.anim_actor_type)
		{
			case AnimatedActorType::PLAYER:
			{
				//figure out current direction based on 
				//difference between current transform position and previous transform position
				
				float xdiff = anim_comp.info.last_position.x - transform.position.x;
				bool south = false; bool north = false;
				bool east = false; bool west = false;
				bool no_move = false;
				
				if(xdiff > 0){west = true;}
				else if(xdiff < 0){east = true;}
				
				float ydiff = anim_comp.info.last_position.y - transform.position.y;
				if(ydiff > 0){north = true;}
				else if(ydiff < 0){south = true;}
				
				if(xdiff == 0 && ydiff == 0){no_move = true;}
				
				anim_comp.info.last_position.y = transform.position.y;
				anim_comp.info.last_position.x = transform.position.x;
				
				//horizontal frame offset refers to horizontal distance between a frame and 
				//the first frame in sprite sheet.
				if(south)
				{
					if(west)
					{
						anim_comp.info.horiz_frame_offset = 3;
					}
					else if(east)
					{
						
					}
					else
					{
						anim_comp.info.horiz_frame_offset = 0;
					}
					
				}
				else if(north)
				{
					
				}
				else
				{
					if(west)
					{
						anim_comp.info.horiz_frame_offset = 6;
					}
				}
				
				//increment frame count
				m_time_counter += dt;
				
				if(m_time_counter >= 0.25f)
				{
					anim_comp.info.frame_count++;
					m_time_counter = 0;
				}
				
				
				//limited to only 3 frames of animation
				if(anim_comp.info.frame_count == 3 || no_move){anim_comp.info.frame_count = 0;}
				
				
				//set render frame based on animation info
				
				//for now only change frames for pants and shirt
				for(size_t i = 0; i < render_comp.multi_render_parts_vec.size(); i++)
				{
					RenderPartDescription& descr = render_comp.multi_render_parts_vec[i].part_description;
					
					if( descr == RenderPartDescription::UPPER_BODY_CLOTHING ||
						descr == RenderPartDescription::LOWER_BODY_CLOTHING || 
						descr == RenderPartDescription::HEAD)
					{
						//change x position of frame selector
						render_comp.multi_render_parts_vec[i].frame_rect.x = (anim_comp.info.frame_count + anim_comp.info.horiz_frame_offset)*anim_comp.info.frame_size;
					}
				}
				
				break;
			}
			
			default:{ break;}
		}
		
		
	}
}


