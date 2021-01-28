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
	
	void SetGamepad1Info(ControllerInput& input_info);
	SDL_Joystick* gGameController;
	SDL_Joystick* gGameController2;
	SDL_Joystick* gGameController3;
	
};

#endif
