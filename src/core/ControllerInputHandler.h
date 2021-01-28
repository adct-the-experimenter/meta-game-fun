#ifndef CONTROLLER_INPUT_HANDLER_H
#define CONTROLLER_INPUT_HANDLER_H

#include "core/ControllerInput.h"
#include "misc/globalvariables.h" //for SDL2 gamepad and data dir path string

class ControllerInputHandler
{
public:
	
	void Init();
	
	void Update(ControllerInput* input);
	
private:
	
	SDL_Event sdl_event; //polls events
	
	void SetGamepadInfo(ControllerInput& input_info);
	
	//game pads
	SDL_Joystick* gGameController;
	SDL_Joystick* gGameController2;
	SDL_Joystick* gGameController3;
	
};

#endif
