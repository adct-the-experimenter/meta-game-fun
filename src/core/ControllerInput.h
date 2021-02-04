#ifndef CONTROLLER_INPUT_H
#define CONTROLLER_INPUT_H

#include "misc/globalvariables.h"
#include <cstdint>

/*

SDL Controller button enums

SDL_CONTROLLER_BUTTON_INVALID
SDL_CONTROLLER_BUTTON_A
SDL_CONTROLLER_BUTTON_B
SDL_CONTROLLER_BUTTON_X
SDL_CONTROLLER_BUTTON_Y
SDL_CONTROLLER_BUTTON_BACK
SDL_CONTROLLER_BUTTON_GUIDE
SDL_CONTROLLER_BUTTON_START
SDL_CONTROLLER_BUTTON_LEFTSTICK
SDL_CONTROLLER_BUTTON_RIGHTSTICK
SDL_CONTROLLER_BUTTON_LEFTSHOULDER
SDL_CONTROLLER_BUTTON_RIGHTSHOULDER
SDL_CONTROLLER_BUTTON_DPAD_UP
SDL_CONTROLLER_BUTTON_DPAD_DOWN
SDL_CONTROLLER_BUTTON_DPAD_LEFT
SDL_CONTROLLER_BUTTON_DPAD_RIGHT
SDL_CONTROLLER_BUTTON_MAX 

*/
										
class ControllerInput
{
public:
	
												
	struct GamepadInfo {
		//left joystick 
		int16_t x_axis;
		int16_t y_axis;
		
		//joystick direction, gets reset for non-continous movement
		int16_t x_dir_axis;
		int16_t y_dir_axis;
		
		//includes buttons and dpad
		SDL_GameControllerButton button;
	};
	
	GamepadInfo gamepad_p1;
	GamepadInfo gamepad_p2;
	GamepadInfo gamepad_p3;
	GamepadInfo gamepad_p4;
	
	void Reset()
	{
		//gamepad_p1.x_axis = 0;
		//gamepad_p1.y_axis = 0;
		gamepad_p1.x_dir_axis = 0;
		gamepad_p1.y_dir_axis = 0;
		gamepad_p1.button =  SDL_CONTROLLER_BUTTON_INVALID ;
		
		gamepad_p2.x_axis = 0;
		gamepad_p2.y_axis = 0;
		gamepad_p2.button =  SDL_CONTROLLER_BUTTON_INVALID ;
		
		gamepad_p3.x_axis = 0;
		gamepad_p3.y_axis = 0;
		gamepad_p3.button =  SDL_CONTROLLER_BUTTON_INVALID ;
		
		gamepad_p4.x_axis = 0;
		gamepad_p4.y_axis = 0;
		gamepad_p4.button =  SDL_CONTROLLER_BUTTON_INVALID ;
	};
};

#endif
