#ifndef CAMERA_H
#define CAMERA_H

#include <cstdint>
#include "raylib.h"

struct CustomCamera
{
	std::uint8_t num_player;
	Rectangle camera;
};

#endif
