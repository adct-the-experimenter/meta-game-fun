#include "PhysicsSystem.h"

#include "core/system.h"

#include "core/coordinator.h"


extern Coordinator gCoordinator;


void PhysicsSystem::Init()
{
	
}

void PhysicsSystem::Update(float& dt)
{
	
	for (auto const& entity : mEntities)
	{
		auto& rigidBody = gCoordinator.GetComponent<RigidBody2D>(entity);
		auto& transform = gCoordinator.GetComponent<Transform2D>(entity);
		auto& physics_type_comp = gCoordinator.GetComponent<PhysicsTypeComponent>(entity);
		
		// Forces
		auto const& gravity = gCoordinator.GetComponent<Gravity2D>(entity);
		
		switch(physics_type_comp.phy_type)
		{
			case PhysicsType::LIFE_RPG:
			{
				//move transform component by velocity of rigid body multiplied by time
				transform.position.x += rigidBody.velocity.x * dt;
				transform.position.y += rigidBody.velocity.y * dt;
				break;
			}
			case PhysicsType::PLATFORMER:
			{
				//account for acceleration due to gravity to rigid body velocity
				rigidBody.velocity.x += gravity.force.x * dt;
				rigidBody.velocity.y += gravity.force.y * dt;
				
				//move transform component by velocity of rigid body multiplied by time
				transform.position.x += rigidBody.velocity.x * dt;
				transform.position.y += rigidBody.velocity.y * dt;
				
				break;
			}
			default:{break;}
		}
		
	}
	
}
