#ifndef SINGLE_RENDER_SYSTEM_H
#define SINGLE_RENDER_SYSTEM_H

#include "systems/RenderSystem.h"

class SingleRenderComponentSystem : public RenderSystem
{
public:
	void Init(std::array <CustomCamera,4> *cameras);

	void Update();
	
private:
	
};

#endif
