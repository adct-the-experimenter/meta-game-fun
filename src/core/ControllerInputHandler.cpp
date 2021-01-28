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

void ControllerInputHandler::Init()
{
	int g_num_players = 2;
	
	//Check for joysticks
	if( SDL_NumJoysticks() < 1 )
	{
		printf( "Warning: No joysticks connected!\n" );
	}
	else
	{
		//Load joystick
		
		//if there is a second player 
		if(g_num_players > 1)
		{
			gGameController = SDL_JoystickOpen( 0 );
			if( gGameController == NULL )
			{
				printf( "Warning: Unable to open game controller 1! SDL Error: %s\n", SDL_GetError() );
			}
			else{std::cout << "Initialized gamepad 1\n";}
			
		}
		
		//if there is a third player
		if(g_num_players > 2)
		{
			gGameController2 = SDL_JoystickOpen( 1 );
			if( gGameController2 == NULL )
			{
				printf( "Warning: Unable to open game controller 2! SDL Error: %s\n", SDL_GetError() );
			}
			else{std::cout << "Initialized gamepad 2\n";}
		}
		
		//if there is a fourth player
		if(g_num_players > 3)
		{
			gGameController3 = SDL_JoystickOpen( 2 );
			if( gGameController2 == NULL )
			{
				printf( "Warning: Unable to open game controller 3! SDL Error: %s\n", SDL_GetError() );
			}
			else{std::cout << "Initialized gamepad 3\n";}
		}
		
	}
	
	std::string mapping_file = DATADIR_STR + "/gamecontrollerdb.txt";
	//SetGamepadMappings(mapping_file.c_str());
	
	SDL_GameControllerAddMappingsFromFile(mapping_file.c_str());
}

void ControllerInputHandler::Update(ControllerInput* input)
{
	ControllerInputHandler::SetGamepadInfo(*input);
}

void ControllerInputHandler::SetGamepadInfo(ControllerInput& input_info)
{
	while( SDL_PollEvent(&sdl_event) != 0)
    {
        if(sdl_event.type == SDL_JOYBUTTONDOWN)
		{
			//gamepad 1
			if(sdl_event.jbutton.which == 0)
			{
							
				if( sdl_event.jbutton.state == SDL_PRESSED )
				{
					 
				}
				else if(sdl_event.jbutton.state == SDL_RELEASED)
				{
					
				}
				
			}
			//gamepad 2
			else if(sdl_event.jbutton.which == 1)
			{
							
				if( sdl_event.jbutton.state == SDL_PRESSED )
				{
				}
				else if(sdl_event.jbutton.state == SDL_RELEASED)
				{
				}
				
			}
			//gamepad 3
			else if(sdl_event.jbutton.which == 2)
			{
				
				if( sdl_event.jbutton.state == SDL_PRESSED )
				{
					
				}
				else if(sdl_event.jbutton.state == SDL_RELEASED)
				{
					
				}
				
			}
			
			
		}
		else if(sdl_event.type == SDL_JOYBUTTONUP)
		{
			//gamepad 1
			if(sdl_event.jbutton.which == 0)
			{
				
				if( sdl_event.jbutton.state == SDL_PRESSED )
				{
				}
				else if(sdl_event.jbutton.state == SDL_RELEASED)
				{
				}
				
			}
			//gamepad 2
			else if(sdl_event.jbutton.which == 1)
			{
				
				if( sdl_event.jbutton.state == SDL_PRESSED )
				{

				}
				else if(sdl_event.jbutton.state == SDL_RELEASED)
				{

				}
				
			}
			//gamepad 3
			else if(sdl_event.jbutton.which == 2)
			{
				
				if( sdl_event.jbutton.state == SDL_PRESSED )
				{

				}
				else if(sdl_event.jbutton.state == SDL_RELEASED)
				{
					
				}
				
			}
			
		}
    }
	
	
}
