
#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#include "core/system.h"

class PhysicsSystem : public System
{
public:
	void Init();

	void Update(float& dt);

private:
	void CheckCollisionWithTilesRPG(float& obj_x, float& obj_y, 
									float& obj_vx, float& obj_vy, 
									float& dt, 
									std::uint32_t& obj_width,
									std::uint32_t& obj_height);
};

#endif
