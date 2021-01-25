#include "core/coordinator.h"
#include "raylib.h"


#include "systems/PhysicsSystem.h"
#include "systems/WorldSystem.h"
#include "systems/RenderSystem.h"
#include "systems/ControllerInputHandler.h"
#include "core/ControllerInput.h"

#include <string>
#include <chrono>
#include <memory>
#include <iostream>

//main

//initialize manager for levels
//initialize list of levels from XML

	
Coordinator gCoordinator;

ControllerInput gControllerInput;
ControllerInputHandler gInputHandler;

std::vector<Entity> entities(MAX_ENTITIES);

//function to initialize main ECS
void InitMainECS();

std::shared_ptr <WorldSystem> worldSystem;
std::shared_ptr <RenderSystem> renderSystem;


//function to initialize video game ECS
void InitVideoGameECS();
std::shared_ptr <PhysicsSystem> physicsSystem;

//function to init raylib system
void InitRaylibSystem();

//function to close raylib system
void CloseRaylibSystem();

//function to run the game loop of event handling, logic render, sound
void GameLoop();

//game loop functions
void handle_events(); //receive input
void logic(); //determine what happens in world based on input
void render(); //draw visual representation of what happens in world to screen
void sound(); //play sounds of audio representation of what happens in world 

bool video_game_playing = false;

int main()
{
	InitRaylibSystem();
	
	InitMainECS();
	
	bool quit = false;
	
	while (!quit)
	{
		// Detect window close button or ESC key
		if(WindowShouldClose())
		{
			quit = true;
		}    
		
		// Main game loop
					
		GameLoop();
			
	}
		
	CloseRaylibSystem();
	
	return 0;
}

void GameLoop()
{
	//handle events through event manager
	handle_events();
	
	//run logic for all entities through systems
	logic();
	
	//run render for all entities in manager
	render();
	
}

void handle_events()
{
	gInputHandler.Update(&gControllerInput);
}

void logic()
{
	
	float dt = GetFrameTime();
	
	worldSystem->Update();
	if(video_game_playing && physicsSystem){physicsSystem->Update(dt);}
	
}

void render()
{
	BeginDrawing();

	ClearBackground(RAYWHITE);
	
	//render time info from world system at top
	std::string time_info = "Day: " + worldSystem->GetDayString() + "  " \
							"Hour: " + std::to_string(worldSystem->GetHours()) + "  " \
							"Minute: " + std::to_string(worldSystem->GetMinutes());
							
	DrawText(time_info.c_str(), 190, 20, 20, LIGHTGRAY);
	
	//renders any entity that has render component
	renderSystem->Update();
	
	EndDrawing();
}

void sound()
{
	
}

void InitMainECS()
{
	//initialize coordinator which initializes entity manager, component manager
	gCoordinator.Init();
	
	//Initialize components for entities
	
	
	gCoordinator.RegisterComponent<Transform2D>(); //id 00000000001
	gCoordinator.RegisterComponent<Player>(); //id 00000000010
	gCoordinator.RegisterComponent<RenderInfo>(); //id 00000000010
	
	//make world system that only reacts to entitties
	//with signature that has player component
	worldSystem = gCoordinator.RegisterSystem<WorldSystem>();
	worldSystem->Init();
	
	Signature sig_world;
	sig_world.set(gCoordinator.GetComponentType<Player>());
	gCoordinator.SetSystemSignature<WorldSystem>(sig_world);
	
	//make rendering system that only reacts to entities
	//with render info component
	renderSystem = gCoordinator.RegisterSystem<RenderSystem>();
	
	Signature sig_render;
	sig_render.set(gCoordinator.GetComponentType<RenderInfo>());
	gCoordinator.SetSystemSignature<RenderSystem>(sig_render);
	
	//make entity for player
	entities[0] = gCoordinator.CreateEntity();
	
	std::uint16_t balance = 200;
	std::uint8_t hp = 100;
	std::string job = "cashier";
	LooksStatus look = LooksStatus::NORMAL;
	PlayerTimeStatus time_stat = PlayerTimeStatus::NONE;
	ActivityStatus activity_stat = ActivityStatus::ROAMING_WORLD;
	gCoordinator.AddComponent(entities[0],Player{.time_status=time_stat,
											.money = balance,
											.health = hp,
											.job_occupation = job,
											.look_status = look,
											.activity_status = activity_stat} );
	Vector2 initP = {2.0f,2.0f};
	gCoordinator.AddComponent(
				entities[0],
				Transform2D{
					.position = initP
				}
			);
	
	Texture2D* texture_player = nullptr;
	Rectangle default_rect = {0,0,40,40};
	gCoordinator.AddComponent(
				entities[0],
				RenderInfo{
					.texture_ptr = texture_player,
					.frame_rect = default_rect
				}
			);
	
}

void InitVideoGameECS()
{
	
	gCoordinator.RegisterComponent<Gravity2D>(); 
	gCoordinator.RegisterComponent<RigidBody2D>();
	
	physicsSystem = gCoordinator.RegisterSystem<PhysicsSystem>();
	
	//make physics system that only reacts to entitities 
	//with signature that has these components
	Signature phys_sys_signature;
	phys_sys_signature.set(gCoordinator.GetComponentType<Gravity2D>());
	phys_sys_signature.set(gCoordinator.GetComponentType<RigidBody2D>());
	phys_sys_signature.set(gCoordinator.GetComponentType<Transform2D>());
	phys_sys_signature.set(gCoordinator.GetComponentType<Player>());
	gCoordinator.SetSystemSignature<PhysicsSystem>(phys_sys_signature);
	
	int it = 0;
	
	//initialize entities with components 
	for (auto& entity : entities)
	{
		entity = gCoordinator.CreateEntity();
		
		//make first entity a player
		if(it > 0)
		{
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
		
		it++;
		
	}
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

