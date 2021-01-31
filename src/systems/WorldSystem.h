#ifndef WORLD_SYSTEM_H
#define WORLD_SYSTEM_H

#include <cstdint>
#include "core/system.h"
#include <chrono>
#include <string>

#include <array>

enum class Day : std::uint8_t {MONDAY=1,TUESDAY,WEDNESDAY,THURSDAY,FRIDAY,SATURDAY,SUNDAY };
enum class Season : std::uint8_t {SUMMER=1,FALL,WINTER,SPRING};
enum class StatusAffected : std::uint8_t {WEALTH=0, HEALTH, LOOKS};

struct LifeEvent
{
	int8_t effect; //-1 loss, 0 nothing, +1 gain
	StatusAffected status_affected;
	std::uint8_t num_player_affected;
	
	std::uint8_t event_day;
	std::uint8_t event_hours;
	bool happened = false;
	
	std::string description;
};

class WorldSystem : public System
{
public:
	void Init();

	void Update();
	
	
	
	std::string GetDayString();
	
	std::uint16_t GetMinutes();
	std::uint8_t GetHours();
	
private:
	
	//time keeping variables
	std::uint8_t m_current_day;
	std::uint8_t m_current_season;
	std::chrono::system_clock::time_point m_start_time;
	std::uint16_t m_minutes;
	std::uint8_t m_hours;
	
	//function to randomly generate life event at certain time of day
	//that will affect a player
	void RandomlyGenerateLifeEvents();
	
	//life events to potentialy happen over 1 week
	std::array <LifeEvent,7> life_events_week;
	
	//function to keep time for hour, day, season
	void KeepTime();
	
	//function to check life events
	void CheckLifeEvents();
	
	//function to get player to enter description a few hours before it happen
	void GetSpecificDescriptionForEvent(LifeEvent& event);
};

#endif
