#include "WorldSystem.h"

#include "core/system.h"

#include "core/coordinator.h"

#include <iostream>

extern Coordinator gCoordinator;


void WorldSystem::Init()
{
	current_day = Day::MONDAY;
	current_season = Season::FALL;
}

void WorldSystem::Update()
{
	for (auto const& entity : mEntities)
	{
		//auto& rigidBody = gCoordinator.GetComponent<RigidBody2D>(entity);
		//auto& transform = gCoordinator.GetComponent<Transform2D>(entity);

		// Forces
		//auto const& gravity = gCoordinator.GetComponent<Gravity2D>(entity);
		
		//get player component from entity
		auto& player = gCoordinator.GetComponent<Player>(entity);
		
		//set player time status
		
		//std::cout << "Player time status is " << int(player.time_status) << std::endl;
		
	}
}
