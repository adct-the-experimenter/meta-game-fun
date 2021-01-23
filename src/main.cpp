#include "coordinator.h"
#include "raylib.h"

#include <chrono>
#include <memory>

#include <iostream>

//main

//initialize manager for levels
//initialize list of levels from XML



//initialize entity manager


//Initialize components for entities

//initialize entities with components 

//initialize systems with entities passed to it

//load life world

//game loop

	//handle events through event manager

	//run logic for all entities through systems

	//run render for all entities in manager

	//stop game loop 
	
Coordinator gCoordinator;

int main()
{
	gCoordinator.Init();

	gCoordinator.RegisterComponent<Gravity2D>();
	gCoordinator.RegisterComponent<RigidBody2D>();
	gCoordinator.RegisterComponent<Transform2D>();

	//auto physicsSystem = gCoordinator.RegisterSystem<PhysicsSystem>();

	Signature signature;
	signature.set(gCoordinator.GetComponentType<Gravity2D>());
	signature.set(gCoordinator.GetComponentType<RigidBody2D>());
	signature.set(gCoordinator.GetComponentType<Transform2D>());
	//gCoordinator.SetSystemSignature<PhysicsSystem>(signature);

	std::vector<Entity> entities(MAX_ENTITIES);

	for (auto& entity : entities)
	{
		entity = gCoordinator.CreateEntity();
		
		Vector2 initGravity = {0.0f,-2.0f};
		
		gCoordinator.AddComponent(
			entity,
			Gravity2D{initGravity}
			);
		
		Vector2 initVel = {0.0f,0.0f};
		Vector2 initAccel = {0.0f,0.0f};
		
		gCoordinator.AddComponent(
			entity,
			RigidBody2D{
				.velocity = initVel,
				.acceleration = initAccel}
				);
		
		Vector2 initP = {2.0f,2.0f};
		
		gCoordinator.AddComponent(
			entity,
			Transform2D{
				.position = initP
			}
		);
	}

	float dt = 0.0f;
	
	bool quit = false;
	
	while (!quit)
	{
		auto startTime = std::chrono::high_resolution_clock::now();

		//physicsSystem->Update(dt);

		auto stopTime = std::chrono::high_resolution_clock::now();

		dt = std::chrono::duration<float, std::chrono::seconds::period>(stopTime - startTime).count();
		
		if(dt != 0){quit = true;}
	}
	
	std::cout << "dt:" << dt << std::endl;
	
}
