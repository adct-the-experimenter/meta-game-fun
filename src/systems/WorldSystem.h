#ifndef WORLD_SYSTEM_H
#define WORLD_SYSTEM_H

#include <cstdint>
#include "core/system.h"
#include <chrono>
#include <string>

class WorldSystem : public System
{
public:
	void Init();

	void Update();
	
	enum class Day : std::uint8_t {MONDAY=1,TUESDAY,WEDNESDAY,THURSDAY,FRIDAY,SATURDAY,SUNDAY };
	enum class Season : std::uint8_t {SUMMER=1,FALL,WINTER,SPRING};
	
	std::string GetDayString();
	
	std::uint16_t GetMinutes();
	std::uint8_t GetHours();
	
private:
	
	std::uint8_t m_current_day;
	std::uint8_t m_current_season;
	std::chrono::system_clock::time_point m_start_time;
	std::uint16_t m_minutes;
	std::uint8_t m_hours;
	
};

#endif
