#include "core/coordinator.h"
#include "raylib.h"

#include <chrono>
#include <memory>

#include <iostream>
#include "systems/PhysicsSystem.h"
#include "systems/WorldSystem.h"

//main

//initialize manager for levels
//initialize list of levels from XML

	
Coordinator gCoordinator;

//function to initialize main ECS
void InitMainECS();

//function to init raylib system
void InitRaylibSystem();

//function to close raylib system
void CloseRaylibSystem();


int main()
{
	InitRaylibSystem();
	//load life world
	
	//initialize coordinator which initializes entity manager, component manager
	gCoordinator.Init();
	
	//Initialize components for entities
	gCoordinator.RegisterComponent<Gravity2D>(); //id 0000000001
	gCoordinator.RegisterComponent<RigidBody2D>(); // id 00000000010
	gCoordinator.RegisterComponent<Transform2D>(); //id 00000000100
	gCoordinator.RegisterComponent<Player>(); //id 00000001000
	
	auto physicsSystem = gCoordinator.RegisterSystem<PhysicsSystem>();
	
	//make physics system that only reacts to entitities 
	//with signature that has these components
	Signature signature;
	signature.set(gCoordinator.GetComponentType<Gravity2D>());
	signature.set(gCoordinator.GetComponentType<RigidBody2D>());
	signature.set(gCoordinator.GetComponentType<Transform2D>());
	gCoordinator.SetSystemSignature<PhysicsSystem>(signature);
	
	//make world system that only reacts to entitties
	//with signature that has player component
	auto worldSystem = gCoordinator.RegisterSystem<WorldSystem>();
	worldSystem->Init();
	
	Signature sig_world;
	sig_world.set(gCoordinator.GetComponentType<Player>());
	gCoordinator.SetSystemSignature<WorldSystem>(sig_world);
	
	//make entities
	std::vector<Entity> entities(MAX_ENTITIES);
	
	int it = 0;
	
	//initialize entities with components 
	for (auto& entity : entities)
	{
		entity = gCoordinator.CreateEntity();
		
		if(it == 0){gCoordinator.AddComponent(entity,Player{PlayerTimeStatus::NONE} );}
		
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
		
		// Main game loop
		while (!WindowShouldClose())    // Detect window close button or ESC key
		{			
			//handle events through event manager

			//run logic for all entities through systems
			dt = GetFrameTime();
			
			physicsSystem->Update(dt);
			worldSystem->Update();
			
			//run render for all entities in manager
			
			BeginDrawing();

			ClearBackground(RAYWHITE);

			DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

			EndDrawing();
			
		}
		
		quit = true;
	}
	
	std::cout << "dt:" << dt << std::endl;
	
	CloseRaylibSystem();
	
	return 0;
}

void InitMainECS()
{
	
}

void InitRaylibSystem()
{

    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Meta Game Fun");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
}

void CloseRaylibSystem()
{
    CloseWindow();        // Close window and OpenGL context
}
