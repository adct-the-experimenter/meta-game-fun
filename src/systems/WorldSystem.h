#ifndef WORLD_SYSTEM_H
#define WORLD_SYSTEM_H

#include <cstdint>
#include "core/system.h"

class WorldSystem : public System
{
public:
	void Init();

	void Update();
	
	enum class Day : std::uint8_t {MONDAY=1,TUESDAY,WEDNESDAY,THURSDAY,FRIDAY,SATURDAY,SUNDAY };
	enum class Season : std::uint8_t {SUMMER=1,FALL,WINTER,SPRING};

private:
	
	Day current_day;
	Season current_season;
	
};

#endif
