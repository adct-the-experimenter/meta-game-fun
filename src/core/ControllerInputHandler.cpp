#include "ControllerInputHandler.h"

#include "raylib.h"
#include <iostream>

#if defined(PLATFORM_RPI)
    #define XBOX360_NAME_ID     "Microsoft X-Box 360 pad"
    #define PS3_NAME_ID         "PLAYSTATION(R)3 Controller"
#else
    #define XBOX360_NAME_ID     "Xbox 360 Controller"
    #define PS3_NAME_ID         "PLAYSTATION(R)3 Controller"
#endif

void ControllerInputHandler::Init(std::uint8_t num_players)
{
	
	m_num_players = num_players;
	
	std::string mapping_file = DATADIR_STR + "/gamecontrollerdb.txt";
	//SetGamepadMappings(mapping_file.c_str());
	
	if( SDL_GameControllerAddMappingsFromFile(mapping_file.c_str()) == -1)
	{
		std::cout << "Failed to add gamepad mappings using SDL2!\n";
	}
	else
	{
		std::cout << "Successfully added gamepad mappings using SDL2!\n";
	}
	
	//Check for joysticks
	if( SDL_NumJoysticks() < 1 )
	{
		printf( "Warning: No joysticks connected!\n" );
	}
	else
	{
		//Load gamepad controllers
		
		//first player 
		if(m_num_players > 0)
		{
			gGameController = SDL_GameControllerOpen( 0 );
			if( gGameController == NULL )
			{
				printf( "Warning: Unable to open game controller 1! SDL Error: %s\n", SDL_GetError() );
			}
			else
			{
				std::cout << "Initialized gamepad 1\n";
			}
			
		}
		
		//if there is a second player
		if(m_num_players > 1)
		{
			gGameController2 = SDL_GameControllerOpen( 1 );
			if( gGameController2 == NULL )
			{
				printf( "Warning: Unable to open game controller 2! SDL Error: %s\n", SDL_GetError() );
			}
			else{std::cout << "Initialized gamepad 2\n";}
		}
		
		//if there is a third player
		if(m_num_players > 2)
		{
			gGameController3 = SDL_GameControllerOpen( 2 );
			if( gGameController2 == NULL )
			{
				printf( "Warning: Unable to open game controller 3! SDL Error: %s\n", SDL_GetError() );
			}
			else{std::cout << "Initialized gamepad 3\n";}
		}
		
	}
	
	
}

void ControllerInputHandler::Update(ControllerInput* input)
{
	input->Reset();
	ControllerInputHandler::SetGamepadInfo(*input);
}

void ControllerInputHandler::SetGamepadInfo(ControllerInput& input_info)
{
	/*
	 * Raylib isn't working for some gamepads even when its defined in game controller mappings.
	 * Need to wait for update or fix for this issue.
	 * Using SDL2 as a workaround.
	 
	if (IsGamepadAvailable(GAMEPAD_PLAYER1))
    {
		 if (GetGamepadButtonPressed() != -1)
		 { 
			 std::cout << "DETECTED BUTTON: " << GetGamepadButtonPressed();
		 }
	}
	*/
	

	while( SDL_PollEvent(&sdl_event) != 0)
    {
        if(sdl_event.type == SDL_CONTROLLERBUTTONDOWN )
		{
			
			//gamepad 1
			if(sdl_event.jbutton.which == 0)
			{
				input_info.gamepads_vec[0].button = SDL_GameControllerButton(sdl_event.jbutton.button);
			}
			//gamepad 2
			else if(sdl_event.jbutton.which == 1)
			{
				input_info.gamepads_vec[1].button = SDL_GameControllerButton(sdl_event.jbutton.button);
			}
			//gamepad 3
			else if(sdl_event.jbutton.which == 2)
			{
				input_info.gamepads_vec[2].button = SDL_GameControllerButton(sdl_event.jbutton.button);	
			}
			
		}
		
		if(sdl_event.type == SDL_CONTROLLERAXISMOTION)
		{
			
			//gamepad 1
			if(sdl_event.jaxis.which == 0)
			{
				if(sdl_event.jaxis.axis == SDL_CONTROLLER_AXIS_LEFTX)
				{
					input_info.gamepads_vec[0].x_axis = sdl_event.jaxis.value;
					input_info.gamepads_vec[0].x_dir_axis = sdl_event.jaxis.value;
				}
				else if(sdl_event.jaxis.axis == SDL_CONTROLLER_AXIS_LEFTY)
				{
					input_info.gamepads_vec[0].y_axis = sdl_event.jaxis.value;
					input_info.gamepads_vec[0].y_dir_axis = sdl_event.jaxis.value;
				}
			}
			//gamepad 2
			else if(sdl_event.jaxis.which == 1)
			{
				if(sdl_event.jaxis.axis == SDL_CONTROLLER_AXIS_LEFTX)
				{
					input_info.gamepads_vec[1].x_axis = sdl_event.jaxis.value;
					input_info.gamepads_vec[1].x_dir_axis = sdl_event.jaxis.value;
				}
				else if(sdl_event.jaxis.axis == SDL_CONTROLLER_AXIS_LEFTY)
				{
					input_info.gamepads_vec[1].y_axis = sdl_event.jaxis.value;
					input_info.gamepads_vec[1].y_dir_axis = sdl_event.jaxis.value;
				}
			}
			//gamepad 3
			else if(sdl_event.jaxis.which == 2)
			{
				if(sdl_event.jaxis.axis == SDL_CONTROLLER_AXIS_LEFTX)
				{
					input_info.gamepads_vec[2].x_axis = sdl_event.jaxis.value;
				}
				else if(sdl_event.jaxis.axis == SDL_CONTROLLER_AXIS_LEFTY)
				{
					input_info.gamepads_vec[2].y_axis = sdl_event.jaxis.value;
				}
			}
		}
    }
	
	
}
