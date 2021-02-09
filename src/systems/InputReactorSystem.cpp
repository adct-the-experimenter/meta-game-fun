#include "InputReactorSystem.h"

#include "core/system.h"
#include "core/coordinator.h"


extern Coordinator gCoordinator;


void InputReactorSystem::Update(ControllerInput& input)
{
	for (auto const& entity : mEntities)
	{
		auto& inputReactor = gCoordinator.GetComponent<InputReact>(entity);
		auto& rigidBody = gCoordinator.GetComponent<RigidBody2D>(entity);
		
		switch(inputReactor.actor_type)
		{
			case InputReactorType::NONE:{break;}
			case InputReactorType::PLAYER:
			{
				//do player specific event handling
				//get other player specific component from entity.
				
				for(size_t i = 0; i < input.gamepads_vec.size(); i++)
				{
					//if player number matches game pad
					if(inputReactor.player_num == i + 1)
					{
						if(input.gamepads_vec[i].x_axis > 16300)
						{
							rigidBody.velocity.x = 100.0f;
						}
						else if(input.gamepads_vec[i].x_axis < -16300)
						{
							rigidBody.velocity.x = -100.0f;
						}
						else 
						{
							rigidBody.velocity.x = 0.0f;
						}
						
						if(input.gamepads_vec[i].y_axis < -16300)
						{
							rigidBody.velocity.y = -100.0f;
						}
						else if(input.gamepads_vec[i].y_axis > 16300)
						{
							rigidBody.velocity.y = 100.0f;
						}
						else 
						{
							rigidBody.velocity.y = 0.0f;
						}
					}
					
				}
				
				
				break;
			}
			case InputReactorType::CAR:{break;}
			default:{break;}
		}
		
	}
}
