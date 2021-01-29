#ifndef CONTROLLER_INPUT_H
#define CONTROLLER_INPUT_H

#include "misc/globalvariables.h"
#include <cstdint>

										
class ControllerInput
{
public:
	
												
	struct GamepadInfo {
		//left joystick 
		int16_t x_axis;
		int16_t y_axis;
		//includes buttons and dpad
		SDL_GameControllerButton button;
	};
	
	GamepadInfo gamepad_p1;
	GamepadInfo gamepad_p2;
	GamepadInfo gamepad_p3;
	GamepadInfo gamepad_p4;
	
	void Reset()
	{
		gamepad_p1.x_axis = 0;
		gamepad_p1.y_axis = 0;
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
