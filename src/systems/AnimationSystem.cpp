#include "AnimationSystem.h"

#include "core/system.h"

#include "core/coordinator.h"

#include <iostream>

extern Coordinator gCoordinator;


void AnimationSystem::Init(std::array <CustomCamera,4> *cameras)
{
	RenderSystem::Init(cameras);
}

void AnimationSystem::Update()
{
	
	for (auto const& entity : mEntities)
	{
		
		if(this->m_cameras_ptr)
		{			
			
			auto& transform = gCoordinator.GetComponent<Transform2D>(entity);
			auto& anim_comp = gCoordinator.GetComponent<Animation>(entity);
			
			switch(anim_comp.anim_actor_type)
			{
				case AnimatedActorType::PLAYER:
				{
					//figure out current direction based on 
					//difference between current transform position and previous transform position
					
					float xdiff = anim_comp.last_position.x - transform.position.x;
					bool south = false; bool north = false;
					bool east = false; bool west = false;
					
					if(xdiff < 0){west = true;}
					else if(xdiff > 0){east = true;}
					
					float ydiff = anim_comp.last_position.y - transform.position.y;
					if(ydiff < 0){north = true;}
					else if(ydiff > 0){south = true;}
					
					//horizontal frame offset refers to horizontal distance between a frame and 
					//the first frame in sprite sheet.
					if(south)
					{
						//frame size is 30
						anim_comp.horiz_frame_offset = 0;
					}
					
					//increment frame count
					anim_comp.frame_count++;
					
					//limited to only 3 frames of animation
					if(anim_comp.frame_count == 3){anim_comp.frame_count = 0;}
					
					break;
				}
				default:{ break;}
			}
		}
		
	}
}


