#include "core/coordinator.h"
#include "raylib.h"


#include "systems/PhysicsSystem.h"
#include "systems/WorldSystem.h"
#include "systems/SingleRenderComponentSystem.h"
#include "systems/MultipleRenderComponentSystem.h"

#include "core/ControllerInputHandler.h"
#include "core/ControllerInput.h"

#include "core/KeyboardTypingInputHandler.h"
#include "core/KeyboardInput.h"

#include "misc/camera.h"
#include "misc/MediaLoader.h"
#include "misc/globalvariables.h"

#include "misc/char_creator.h" //for CharacterCreator class

#include <string>
#include <chrono>
#include <memory>
#include <iostream>
#include <array>

//main

//initialize manager for levels
//initialize list of levels from XML

	
Coordinator gCoordinator;

ControllerInput gControllerInput;
ControllerInputHandler gControllerInputHandler;

KeyboardInput gKeyboardInput;
KeyboardTypingInputHandler gKeyboardTypingInputHandler;

std::vector <Entity> entities(MAX_ENTITIES);

//function to initialize main ECS
void InitMainECS();

std::shared_ptr <WorldSystem> worldSystem;
std::shared_ptr <SingleRenderComponentSystem> single_comp_renderSystem;
std::shared_ptr <MultipleRenderComponentSystem> multi_comp_renderSystem;


//function to initialize video game ECS
void InitVideoGameECS();
std::shared_ptr <PhysicsSystem> physicsSystem;

//function to init raylib system
void InitRaylibSystem();

//function to close raylib system
void CloseRaylibSystem();

//function to load media for game
bool loadMedia();
MediaLoader gMediaLoader;

//function to run the game loop of event handling, logic render, sound
void GameLoop();

//game loop functions
void handle_events(); //receive input
void logic(); //determine what happens in world based on input
void render(); //draw visual representation of what happens in world to screen
void sound(); //play sounds of audio representation of what happens in world 

//game state
enum class GameState : std::uint8_t {TITLE_MENU=0, CHAR_CREATOR, GAME};
GameState m_game_state = GameState::TITLE_MENU;

//camera to follow players.
std::array <CustomCamera,4> player_cameras; 

bool video_game_playing = false;

CharacterCreator gCharCreator;

int main()
{
	InitRaylibSystem();
	
	
	if(!loadMedia())
	{
		std::cout << "Not loading game. Failed to load media!\n";
	}
	else
	{
		InitMainECS();
		
		gCharCreator.Init(&entities,1);
		
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
	}
	
	gMediaLoader.freeMedia();
	
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
	gControllerInputHandler.Update(&gControllerInput);
	
	gKeyboardTypingInputHandler.Update(&gKeyboardInput);
	
	switch(m_game_state)
	{
		case GameState::TITLE_MENU:
		{
			//if a is pressed by gamepad 1
			if(gControllerInput.gamepad_p1.button == SDL_CONTROLLER_BUTTON_A)
			{
				//move to next state
				m_game_state = GameState::CHAR_CREATOR;
			}
			break;
		}
		case GameState::CHAR_CREATOR:
		{
			//run logic for character creator system here
			gCharCreator.handle_input(gControllerInput,gKeyboardInput);
			break;
		}
		case GameState::GAME:
		{	
			break;
		}
	}
}

void logic()
{
	
	float dt = GetFrameTime();
	
	//game
	
	switch(m_game_state)
	{
		case GameState::TITLE_MENU:
		{
			break;
		}
		case GameState::CHAR_CREATOR:
		{
			//run logic for character creator system here
			gCharCreator.logic();
			
			if(gCharCreator.MoveToNextStateBool())
			{
				m_game_state = GameState::GAME;
			}
			
			break;
		}
		case GameState::GAME:
		{
			worldSystem->Update();
			
			if(video_game_playing && physicsSystem){physicsSystem->Update(dt);}
			
			break;
		}
	}
}

void render()
{
	BeginDrawing();

	ClearBackground(RAYWHITE);
	
	switch(m_game_state)
	{
		case GameState::TITLE_MENU:
		{
			DrawTexture(title_menu_texture, 0, 0, WHITE);
			DrawText("Press A on controller to start.", 190, 320, 20, LIGHTGRAY);
			
			break;
		}
		case GameState::CHAR_CREATOR:
		{
			DrawText("In character creator.Press A to finish character creation.", 80, 20, 20, BLACK);
			gCharCreator.render();
			break;
		}
		case GameState::GAME:
		{
			//render time info from world system at top
			std::string time_info = "Day: " + worldSystem->GetDayString() + "  " \
									"Hour: " + std::to_string(worldSystem->GetHours()) + "  " \
									"Minute: " + std::to_string(worldSystem->GetMinutes());
									
			DrawText(time_info.c_str(), 190, 20, 20, LIGHTGRAY);
		    //renders any entity that has render component
			single_comp_renderSystem->Update();	
			multi_comp_renderSystem->Update();	
			
			break;
		}
	}
	
	
	
	EndDrawing();
}

void sound()
{
	
}

bool loadMedia()
{
	bool success = true;
	
	if( !gMediaLoader.loadMedia() )
	{
		success = false;
	}
	
	return success;
}

void InitMainECS()
{
	//initialize coordinator which initializes entity manager, component manager
	gCoordinator.Init();
	
	//Initialize components for entities
	
	gCoordinator.RegisterComponent<Transform2D>(); //id 00000000001
	gCoordinator.RegisterComponent<Player>(); //id 00000000010
	gCoordinator.RegisterComponent<RenderInfo>(); //id 00000000010
	gCoordinator.RegisterComponent<SingleRenderComponent>();
	gCoordinator.RegisterComponent<MultipleRenderComponent>();
	
	//make world system that only reacts to entitties
	//with signature that has player component
	worldSystem = gCoordinator.RegisterSystem<WorldSystem>();
	worldSystem->Init();
	
	Signature sig_world;
	sig_world.set(gCoordinator.GetComponentType<Player>());
	gCoordinator.SetSystemSignature<WorldSystem>(sig_world);
	
	//make rendering system that only reacts to entities
	//with render info component
	single_comp_renderSystem = gCoordinator.RegisterSystem<SingleRenderComponentSystem>();
	single_comp_renderSystem->Init(&player_cameras);
	
	Signature sig_render_single;
	sig_render_single.set(gCoordinator.GetComponentType<SingleRenderComponent>());
	gCoordinator.SetSystemSignature<SingleRenderComponentSystem>(sig_render_single);
	
	multi_comp_renderSystem = gCoordinator.RegisterSystem<MultipleRenderComponentSystem>();
	multi_comp_renderSystem->Init(&player_cameras);
	
	Signature sig_render_multi;
	sig_render_multi.set(gCoordinator.GetComponentType<MultipleRenderComponent>());
	gCoordinator.SetSystemSignature<MultipleRenderComponentSystem>(sig_render_multi);
	
	
	//make entity for player
	entities[0] = gCoordinator.CreateEntity();
	
	std::string name = "Something";
	std::string partner_name = "none";
	std::uint16_t balance = 200;
	std::uint8_t hp = 100;
	std::string job = "cashier";
	LooksStatus look = LooksStatus::NORMAL;
	PlayerTimeStatus time_stat = PlayerTimeStatus::NONE;
	ActivityStatus activity_stat = ActivityStatus::ROAMING_WORLD;
	gCoordinator.AddComponent(entities[0],Player{.name = name, .romantic_partner_name = partner_name,
											.time_status=time_stat,
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
    const int screenHeight = 600;
	
	SetConfigFlags(FLAG_MSAA_4X_HINT);  // Set MSAA 4X hint before windows creation
	
    InitWindow(screenWidth, screenHeight, "Meta Game Fun");
	
	
	// initialize SDL2 for gamepad handling
	if( SDL_Init( SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER) < 0 )
	{
		printf( "SDL input could not initialize! SDL Error: %s\n", SDL_GetError() );
	}

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    
    //initialize game controller input
    gControllerInputHandler.Init();
}

void CloseRaylibSystem()
{
    CloseWindow();        // Close window and OpenGL context
}

