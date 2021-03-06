#include "core/coordinator.h"
#include "raylib.h"

#include "systems/InputReactorSystem.h"

#include "systems/PhysicsSystem.h"
#include "systems/WorldSystem.h"

#include "systems/CameraSystem.h"
#include "systems/RenderSystem.h"
#include "systems/AnimationSystem.h"

#include "core/ControllerInputHandler.h"
#include "core/ControllerInput.h"

#include "core/KeyboardTypingInputHandler.h"
#include "core/KeyboardInput.h"

#include "misc/camera.h"
#include "misc/MediaLoader.h"
#include "misc/globalvariables.h"

#include "misc/TileManager.h"

#include "misc/num_player_setter.h" //for NumPlayerSetter class
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
std::shared_ptr <RenderSystem> renderSystem;
std::shared_ptr <AnimationSystem> animationSystem;

std::shared_ptr <InputReactorSystem> input_ReactSystem;


std::shared_ptr <PhysicsSystem> physicsSystem;

//function to initialize video game ECS
void InitVideoGameECS();

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
std::shared_ptr <CameraSystem> cameraSystem;
std::vector <CustomCamera> player_cameras; 

bool video_game_playing = false;

CharacterCreator gCharCreator;
NumPlayerSetter gNumPlayerSetter;
std::int8_t gNumPlayers = 0;

const std::int16_t screenWidth = 800;
const std::int16_t screenHeight = 600;

TileManager gTileManager;

int main(int argc, char* args[])
{
	InitRaylibSystem();
	
	
	if(!loadMedia())
	{
		std::cout << "Not loading game. Failed to load media!\n";
	}
	else
	{
	
		gControllerInput.Init(1);
		gNumPlayerSetter.Init();
		
		InitMainECS();
		
		//if first level loaded well
		if(gTileManager.LoadLevel(0))
		{
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
		else
		{
			std::cout << "Failed to load first level. Quitting.";
		}
		
	}
	
	renderSystem->UnloadTexture();
	gMediaLoader.freeMedia();
	gTileManager.FreeLevels();
	
    
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
			gNumPlayerSetter.handle_input(gControllerInput,gKeyboardInput);
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
			worldSystem->handle_events(gKeyboardInput);
			
			input_ReactSystem->Update(gControllerInput);
				
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
			gNumPlayerSetter.logic();
			
			//if need to move to next state
			if(gNumPlayerSetter.MoveToNextStateBool())
			{
				//create entities for player
				gNumPlayerSetter.CreatePlayerEntities(&entities);
				
				gNumPlayers = gNumPlayerSetter.GetNumberOfPlayers();
				
				//reset to new number of players for input handling
				gControllerInput.Init(gNumPlayers);
				gControllerInputHandler.Init(gNumPlayers);
				
				//initialze char creator
				gCharCreator.Init(&entities,gNumPlayers);
				
				//initialize camera system
				player_cameras.resize(gNumPlayers);
				cameraSystem->Init(&player_cameras,gNumPlayers,screenWidth,screenHeight);
				
				//initialize render system
				renderSystem->Init(&player_cameras,gNumPlayers);
				
				//move to next state
				m_game_state = GameState::CHAR_CREATOR;
			}
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
			
			physicsSystem->Update(dt);
			
			//set up frame for render
			animationSystem->Update(dt);
			
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
			
			gNumPlayerSetter.render();
			
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
			
			cameraSystem->Update();
			
		    //render any entity that has render component
			renderSystem->Update();
			
			worldSystem->render();
			
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
	
	gCoordinator.RegisterComponent<Transform2D>(); //id 1
	gCoordinator.RegisterComponent<RigidBody2D>();
	gCoordinator.RegisterComponent<Player>();
	gCoordinator.RegisterComponent<RenderInfo>();
	gCoordinator.RegisterComponent<RenderComponent>();
	gCoordinator.RegisterComponent<RenderPosition>();
	gCoordinator.RegisterComponent<InputReact>();
	gCoordinator.RegisterComponent<PhysicsTypeComponent>();
	gCoordinator.RegisterComponent<Animation>();
	gCoordinator.RegisterComponent<CollisionBox>();
	
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
	sig_render.set(gCoordinator.GetComponentType<RenderComponent>());
	sig_render.set(gCoordinator.GetComponentType<Transform2D>());
	sig_render.set(gCoordinator.GetComponentType<RenderPosition>());
	gCoordinator.SetSystemSignature<RenderSystem>(sig_render);
	
	//make input reactor system that only reacts to components input react and transform
	input_ReactSystem = gCoordinator.RegisterSystem<InputReactorSystem>();
	
	Signature sig_input_react;
	sig_input_react.set(gCoordinator.GetComponentType<InputReact>());
	sig_input_react.set(gCoordinator.GetComponentType<RigidBody2D>());
	gCoordinator.SetSystemSignature<InputReactorSystem>(sig_input_react);
	
	//make physics system that only reacts to entitities 
	//with signature that has these components
	
	gCoordinator.RegisterComponent<Gravity2D>(); 
	
	physicsSystem = gCoordinator.RegisterSystem<PhysicsSystem>();
	
	
	Signature phys_sys_signature;
	phys_sys_signature.set(gCoordinator.GetComponentType<Gravity2D>());
	phys_sys_signature.set(gCoordinator.GetComponentType<RigidBody2D>());
	phys_sys_signature.set(gCoordinator.GetComponentType<Transform2D>());
	phys_sys_signature.set(gCoordinator.GetComponentType<PhysicsTypeComponent>());
	phys_sys_signature.set(gCoordinator.GetComponentType<CollisionBox>());
	gCoordinator.SetSystemSignature<PhysicsSystem>(phys_sys_signature);
	
	//make camera system that only reacts to entities
	//with signature that has these components
	
	cameraSystem = gCoordinator.RegisterSystem<CameraSystem>();
	
	Signature camera_sig;
	camera_sig.set(gCoordinator.GetComponentType<Transform2D>());
	camera_sig.set(gCoordinator.GetComponentType<RenderPosition>());
	camera_sig.set(gCoordinator.GetComponentType<Player>());
	gCoordinator.SetSystemSignature<CameraSystem>(camera_sig);
	
	
	//make animation system that only reacts to entities
	//with signature that has these components
	
	animationSystem = gCoordinator.RegisterSystem<AnimationSystem>();
	animationSystem->Init();
	
	Signature animation_sig;
	animation_sig.set(gCoordinator.GetComponentType<Transform2D>());
	animation_sig.set(gCoordinator.GetComponentType<Animation>());
	animation_sig.set(gCoordinator.GetComponentType<RenderComponent>());
	gCoordinator.SetSystemSignature<AnimationSystem>(animation_sig);
	
}

void InitVideoGameECS()
{
	

	
}

void InitRaylibSystem()
{
	
	SetConfigFlags(FLAG_MSAA_4X_HINT);  // Set MSAA 4X hint before windows creation
	
    InitWindow(screenWidth, screenHeight, "Meta Game Fun");
	
	
	// initialize SDL2 for gamepad handling
	if( SDL_Init( SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER) < 0 )
	{
		printf( "SDL input could not initialize! SDL Error: %s\n", SDL_GetError() );
	}

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    
    //initialize game controller input
    gControllerInputHandler.Init(1);
}

void CloseRaylibSystem()
{
    CloseWindow();        // Close window and OpenGL context
    
    //Quit SDL subsystems
    SDL_Quit();
}

