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
	ControllerInputHandler::SetGamepad1Info(*input);
}

void ControllerInputHandler::SetGamepad1Info(ControllerInput& input_info)
{
	/*
	if(sdl_event_ptr->type == SDL_JOYBUTTONDOWN)
	{
		if(sdl_event_ptr->jbutton.which == 0)
		{
			player_num = 2;
			
			if( sdl_event_ptr->jbutton.state == SDL_PRESSED )
			{
				thisEvent.event_id = Event_ID::JOYSTICK_BUTTON_DOWN_PRESSED; 
			}
			else if(sdl_event_ptr->jbutton.state == SDL_RELEASED)
			{
				thisEvent.event_id = Event_ID::JOYSTICK_BUTTON_DOWN_RELEASED;
			}
			
			pushEventInstance(thisEvent,player_num);
		}
		else if(sdl_event_ptr->jbutton.which == 1)
		{
			player_num = 3;
			
			if( sdl_event_ptr->jbutton.state == SDL_PRESSED )
			{
				thisEvent.event_id = Event_ID::JOYSTICK_BUTTON_DOWN_PRESSED; 
			}
			else if(sdl_event_ptr->jbutton.state == SDL_RELEASED)
			{
				thisEvent.event_id = Event_ID::JOYSTICK_BUTTON_DOWN_RELEASED;
			}
			
			pushEventInstance(thisEvent,player_num);
		}
		else if(sdl_event_ptr->jbutton.which == 2)
		{
			player_num = 4;
			
			if( sdl_event_ptr->jbutton.state == SDL_PRESSED )
			{
				thisEvent.event_id = Event_ID::JOYSTICK_BUTTON_DOWN_PRESSED; 
			}
			else if(sdl_event_ptr->jbutton.state == SDL_RELEASED)
			{
				thisEvent.event_id = Event_ID::JOYSTICK_BUTTON_DOWN_RELEASED;
			}
			
			pushEventInstance(thisEvent,player_num);
		}
		
		
	}
	else if(sdl_event_ptr->type == SDL_JOYBUTTONUP)
	{
		if(sdl_event_ptr->jbutton.which == 0)
		{
			player_num = 2;
			
			if( sdl_event_ptr->jbutton.state == SDL_PRESSED )
			{
				thisEvent.event_id = Event_ID::JOYSTICK_BUTTON_UP_PRESSED; 
			}
			else if(sdl_event_ptr->jbutton.state == SDL_RELEASED)
			{
				thisEvent.event_id = Event_ID::JOYSTICK_BUTTON_UP_RELEASED;
			}
			
			pushEventInstance(thisEvent,player_num);
		}
		else if(sdl_event_ptr->jbutton.which == 1)
		{
			player_num = 3;
			
			if( sdl_event_ptr->jbutton.state == SDL_PRESSED )
			{
				thisEvent.event_id = Event_ID::JOYSTICK_BUTTON_UP_PRESSED; 
			}
			else if(sdl_event_ptr->jbutton.state == SDL_RELEASED)
			{
				thisEvent.event_id = Event_ID::JOYSTICK_BUTTON_UP_RELEASED;
			}
			
			pushEventInstance(thisEvent,player_num);
		}
		else if(sdl_event_ptr->jbutton.which == 2)
		{
			player_num = 4;
			
			if( sdl_event_ptr->jbutton.state == SDL_PRESSED )
			{
				thisEvent.event_id = Event_ID::JOYSTICK_BUTTON_UP_PRESSED; 
			}
			else if(sdl_event_ptr->jbutton.state == SDL_RELEASED)
			{
				thisEvent.event_id = Event_ID::JOYSTICK_BUTTON_UP_RELEASED;
			}
			
			pushEventInstance(thisEvent,player_num);
		}
		
	}
	
	*/
}
