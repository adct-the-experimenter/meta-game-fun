#include "InputReactorSystem.h"

#include "core/system.h"

#include "core/coordinator.h"


extern Coordinator gCoordinator;


void InputReactorSystem::Update(ControllerInput& input)
{
	for (auto const& entity : mEntities)
	{
		auto& inputReactor = gCoordinator.GetComponent<InputReact>(entity);
		auto& transform = gCoordinator.GetComponent<Transform2D>(entity);
		
		
		switch(inputReactor.actor_type)
		{
			case InputReactorType::NONE:{break;}
			case InputReactorType::PLAYER:{break;}
			case InputReactorType::CAR:{break;}
			default:{break;}
		}
		
		// Forces
		//auto const& gravity = gCoordinator.GetComponent<Gravity2D>(entity);
		
		//move transform component by velocity of rigid body multiplied by time
		//transform.position.x += rigidBody.velocity.x * dt;
		//transform.position.y += rigidBody.velocity.y * dt;
		
		//account for acceleration due to gravity to rigid body velocity
		//rigidBody.velocity.x += gravity.force.x * dt;
		//rigidBody.velocity.y += gravity.force.y * dt;
	}
}
