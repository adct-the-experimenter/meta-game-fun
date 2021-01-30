#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include <cstdint>
#include "core/system.h"

#include <array>
#include "misc/camera.h"

class RenderSystem : public System
{
public:
	void Init(std::array <CustomCamera,4> *cameras);

	void Update();
	
protected:
	std::array <CustomCamera,4> *m_cameras_ptr;
	
};

#endif
