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

#include "core/MouseInputHandler.h"
#include "core/MouseInput.h"

#include "misc/camera.h"
#include "misc/MediaLoader.h"
#include "misc/globalvariables.h"

#include "misc/num_player_setter.h" //for NumPlayerSetter class

#include <string>
#include <chrono>
#include <memory>
#include <iostream>
#include <array>

#include "tile_editor.h"

#include <fstream>

//main

//initialize manager for levels
//initialize list of levels from XML

	
Coordinator gCoordinator;

ControllerInput gControllerInput;
ControllerInputHandler gControllerInputHandler;

KeyboardInput gKeyboardInput;
KeyboardTypingInputHandler gKeyboardTypingInputHandler;

MouseInput gMouseInput;
MouseInputHandler gMouseInputHandler;

std::vector <Entity> entities(MAX_ENTITIES);

//function to initialize main ECS
void InitMainECS();

std::shared_ptr <RenderSystem> renderSystem;
std::shared_ptr <AnimationSystem> animationSystem;

std::shared_ptr <InputReactorSystem> input_ReactSystem;


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
std::shared_ptr <CameraSystem> cameraSystem;
std::vector <CustomCamera> player_cameras; 

bool video_game_playing = false;

NumPlayerSetter gNumPlayerSetter;
std::int8_t gNumPlayers = 0;

const std::uint16_t screenWidth = 800;
const std::uint16_t screenHeight = 600;

std::uint32_t levelWidth;
std::uint32_t levelHeight;

TileEditor gTileEditor;
std::string tilesheet_descr_xml;
std::string map_file_xml;
bool createNewMapFile;

int CheckConsoleArgs(int argc, char* argv[]);

int main(int argc, char* argv[])
{
	
	
	if( CheckConsoleArgs(argc,argv) == 0)
	{
		return 0;
	};
	
	InitRaylibSystem();
	
	//initialize tile editor
	
	//load tile editor resources
	
	if(!loadMedia())
	{
		std::cout << "Not loading game. Failed to load media!\n";
	}
	else
	{
		gTileEditor.SetSaveXMLFile(map_file_xml);
		
		bool levelInitialized = true;
			
		if(createNewMapFile)
		{
			if(!gTileEditor.LoadDataBasedOnTilesheetDescription(tilesheet_descr_xml))
			{
				std::cout << "Failed to load data from tilesheet description!\n";
				gMediaLoader.freeMedia();
				CloseRaylibSystem();
				return 0;
			}
			
			gTileEditor.SetLevelDimensions(30,30,levelWidth,levelHeight);
			
		}
		else
		{
			if(!gTileEditor.LoadDataFromXMLFile(map_file_xml,tilesheet_descr_xml))
			{
				std::cout << "Failed to load data from tilesheet description!\n";
				gMediaLoader.freeMedia();
				CloseRaylibSystem();
				return 0;
			}
		}
		
		gControllerInput.Init(1);
		gNumPlayerSetter.Init();
	
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
	}
	
	renderSystem->UnloadTexture();
	gMediaLoader.freeMedia();
	
	
    //Quit SDL subsystems
    SDL_Quit();
    
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
	
	gMouseInputHandler.Update(&gMouseInput);
	
	switch(m_game_state)
	{
		case GameState::TITLE_MENU:
		{
			gNumPlayerSetter.handle_input(gControllerInput,gKeyboardInput);
			break;
		}
		case GameState::GAME:
		{
			gTileEditor.handleInputMouse(gMouseInput);			
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
			//Intialize players here
			for(size_t i = 0; i < gNumPlayers; i++)
			{
				entities[i] = gCoordinator.CreateEntity();
				
				Texture2D* texture_player = &rpg_sprite_sheet_texture;
				
				std::array <RenderPartDescription,6> descript_array = {RenderPartDescription::HAIR,
																		RenderPartDescription::HEAD,
																		RenderPartDescription::EYE,
																		RenderPartDescription::UPPER_BODY_CLOTHING,
																		RenderPartDescription::LOWER_BODY_CLOTHING,
																		RenderPartDescription::SHOES};
				std::vector <RenderInfo> temp_body_parts;
				temp_body_parts.resize(6);
				
				for(size_t slot_it = 0; slot_it < 6; slot_it++)
				{
					temp_body_parts[slot_it].texture_ptr = texture_player;
					temp_body_parts[slot_it].frame_rect = {0,slot_it*30,30,30};
					temp_body_parts[slot_it].tint = WHITE;
					temp_body_parts[slot_it].part_description = descript_array[slot_it];
					temp_body_parts[slot_it].position = {2.0f,2.0f};
				}
				
				temp_body_parts[0].position = {5.0f,10.0f};
				temp_body_parts[1].position = {5.0f,15.0f};
				temp_body_parts[2].position = {5.0f,17.0f};
				temp_body_parts[3].position = {5.0f,40.0f};
				temp_body_parts[4].position = {5.0f,65.0f};
				temp_body_parts[5].position = {5.0f,75.0f};
				
				
				
				gCoordinator.AddComponent(
								entities[i],
								RenderComponent{
									.multi_render_parts_vec = temp_body_parts,
									.multi_part = true
								}
							);
				
				Vector2 char_position = {40,40};
				
				//add render position
				gCoordinator.AddComponent(
								entities[i],
								RenderPosition{
									.overall_position = char_position
								}
							);
				
				
				//add player info component
				
				std::string name = "Player" + std::to_string(i+1);
				std::string partner_name = "none";
				std::uint16_t balance = 200;
				std::uint8_t hp = 100;
				std::string job = "none";
				LooksStatus look = LooksStatus::NORMAL;
				PlayerTimeStatus time_stat = PlayerTimeStatus::NONE;
				ActivityStatus activity_stat = ActivityStatus::ROAMING_WORLD;
				gCoordinator.AddComponent(entities[i], Player{.name = name, .romantic_partner_name = partner_name,
														.time_status=time_stat,
														.money = balance,
														.health = hp,
														.job_occupation = job,
														.look_status = look,
														.activity_status = activity_stat,
														.num_player = i + 1} );
														
				//add input react component
				InputReact react;
				react.actor_type = InputReactorType::PLAYER;
				react.reactToInput = true;
				react.player_num = i + 1;
								 
				gCoordinator.AddComponent(
								entities[i],
								react
								);
				
				//add transform
				Vector2 initP = {2.0f,2.0f};
				gCoordinator.AddComponent(
							entities[i],
							Transform2D{
								.position = initP
							}
						);
				
				//add rigid body
				Vector2 initV = {0.0f,0.0f};
				gCoordinator.AddComponent(
							entities[i],
							RigidBody2D{
								.velocity = initV
							}
						);
						
				//add gravity component for later use
				Vector2 grav = {0.0f,0.0f};
				gCoordinator.AddComponent(
							entities[i],
							Gravity2D{
								.force = grav 
							}
						);
						
				//add physics type
				PhysicsType pType = PhysicsType::LIFE_RPG;
				gCoordinator.AddComponent(
							entities[i],
							PhysicsTypeComponent{
								.phy_type = pType 
							}
						);
						
				//add animation component
				AnimationInfo aInfo;
				aInfo.anim_actor_type = AnimatedActorType::PLAYER;
				aInfo.horiz_frame_offset = 0;
				aInfo.frame_size = 30;
				aInfo.frame_count = 0;
				gCoordinator.AddComponent(
							entities[i],
							Animation{
								.info = aInfo
							}
						);
				
				//add collision box
				std::uint32_t width = 30;
				std::uint32_t height = 90;
				
				gCoordinator.AddComponent(
										entities[i],
										CollisionBox{
											.width = width,
											.height = height
										});
			}
			
			//move to next state
			m_game_state = GameState::GAME;
			break;
		}
		case GameState::GAME:
		{
			gTileEditor.logic(player_cameras[0].camera_rect);			
			
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
		case GameState::GAME:
		{
			cameraSystem->Update();
			
		    //render any entity that has render component
			renderSystem->Update();
			
			gTileEditor.render();
			
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
	
    InitWindow(screenWidth, screenHeight, "Tile Editor");
	
	
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
}

int CheckConsoleArgs(int argc, char* argv[])
{
	for (size_t i = 1; i < argc; ++i) 
	{
		//if path to tile sheet description is given
		if(std::string(argv[i]) == "--tsd")
		{
			if (i + 1 < argc) 
			{ 
				// Make sure we aren't at the end of argv!
				tilesheet_descr_xml = std::string(argv[i+1]); // Increment 'i' so we don't get the argument as the next argv[i].
				
				if(tilesheet_descr_xml == "")
				{
					std::cout << "Put in an xml file describing layout of frames tile sheet.\
					\n Example: ./tile-editor --tsd something.xml";
				}
					
				std::ifstream ifile(tilesheet_descr_xml);
				if((bool)ifile)
				{
					std::cout << "Using " << tilesheet_descr_xml << std::endl;
				}
				else
				{
					std::cout << "Failed to find file " << tilesheet_descr_xml << ". It does not exist!\n";
					return 0;
				}

			}
		}
		//if path to already created xml file is givenn
		else if(std::string(argv[i]) == "--map_file")
		{
			if (i + 1 < argc) 
			{ 
				// Make sure we aren't at the end of argv!
				map_file_xml = std::string(argv[i+1]); // Increment 'i' so we don't get the argument as the next argv[i].
				
				if(map_file_xml == "")
				{
					std::cout << "Put in a map file describing layour of tiles.\
					\n Example: ./tile-editor --map_file something.xml";
				}
					
				std::ifstream ifile(map_file_xml);
				if((bool)ifile)
				{
					std::cout << "Using " << map_file_xml << std::endl;
					createNewMapFile = false;;
				}
				else
				{
					std::cout << "Failed to find file " << map_file_xml << ". It does not exist!\n";
					std::cout << "Creating new file " << map_file_xml << std::endl;
					createNewMapFile = true;
				}

			}
		}
		else if(std::string(argv[i]) == "--levelwidth")
		{
			if (i + 1 < argc) 
			{ 
				// Make sure we aren't at the end of argv!
				levelWidth = atoi( argv[i+1] ); // Increment 'i' so we don't get the argument as the next argv[i].
			}
		}
		else if(std::string(argv[i]) == "--levelheight")
		{
			if (i + 1 < argc) 
			{ 
				// Make sure we aren't at the end of argv!
				levelHeight = atoi( argv[i+1] ); // Increment 'i' so we don't get the argument as the next argv[i].
			}
		}
		//else if help is called
		else if(std::string(argv[i]) == "--help")
		{
			std::cout << "\nHelp for Tile Editor\n\n\tflag file \tdescription\n\t--tsd tsd.xml \tSet tilesheet description\n\t--map_file map.xml \tSet file to load and/or save from.\n";
			return 0;
		}
	}
	
	return 1;
}
