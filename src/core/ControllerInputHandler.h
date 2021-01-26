#ifndef CONTROLLER_INPUT_HANDLER_H
#define CONTROLLER_INPUT_HANDLER_H

#include "core/ControllerInput.h"

class ControllerInputHandler
{
public:
	void Update(ControllerInput* input);
	
private:
	
	void SetGamepad1Info(ControllerInput& input_info);
	
};

#endif
