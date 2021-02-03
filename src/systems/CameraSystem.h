#ifndef CAMERA_SYSTEM_H
#define CAMERA_SYSTEM_H

#include <cstdint>
#include "core/system.h"

#include <array>
#include "misc/camera.h"

class CameraSystem : public System
{
public:
	void Init(std::array <CustomCamera,4> *cameras, std::uint8_t num_players,
				std::uint16_t screenWidth, std::uint16_t screenHeight);

	void Update();
	
protected:

	std::array <CustomCamera,4> *m_cameras_ptr;
	std::uint8_t m_num_players;
};

#endif
