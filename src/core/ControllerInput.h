#ifndef CONTROLLER_INPUT_H
#define CONTROLLER_INPUT_H

#include <cstdint>

enum class DPadDirection : std::uint8_t {NONE=0,UP,DOWN,LEFT,RIGHT,
												UP_LEFT, UP_RIGHT,
												DOWN_LEFT,DOWN_RIGHT};

enum class DButton : std::uint8_t {NONE=0, TOP,LEFT,RIGHT,DOWN};
										
class ControllerInput
{
public:
	
												
	struct GamepadInfo {
		DPadDirection dpad_dir;
		float x_axis;
		float y_axis;
		std::uint8_t button;
	};
	
	GamepadInfo gamepad_p1;
	GamepadInfo gamepad_p2;
	GamepadInfo gamepad_p3;
	GamepadInfo gamepad_p4;
};

#endif
