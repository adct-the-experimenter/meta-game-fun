#include "WorldSystem.h"

#include "core/system.h"

#include "core/coordinator.h"

#include <iostream>

extern Coordinator gCoordinator;


void WorldSystem::Init()
{
	m_current_day = std::uint8_t(Day::MONDAY);
	m_current_season = std::uint8_t(Season::FALL);
	
	m_start_time = std::chrono::system_clock::now();
	
	m_minutes = 0;
	m_hours = 0;
	
	//randomly generate this life events for the week
	WorldSystem::RandomlyGenerateLifeEvents();
}

void WorldSystem::Update()
{
	KeepTime();
	
	for (auto const& entity : mEntities)
	{
		
		//get player component from entity
		auto& player = gCoordinator.GetComponent<Player>(entity);
		
		if(player.health)
		{
			//set player time status based on hour
			switch(player.time_status)
			{
				case PlayerTimeStatus::NONE:{break;}
				default:{break;}
			}
			
			//if current time matches life event time and it has not occurred yet
		}
		
	}
}

void WorldSystem::KeepTime()
{
	//update time of day for free time period
	auto current_time = std::chrono::system_clock::now();
	
	//if a real minute has passed since time started or reset
	double seconds = std::chrono::duration_cast<std::chrono::seconds>(current_time - m_start_time).count();
	
	if( seconds >= 10) 
	{
		//update game minutes by 10
		m_minutes += 1;
		//reset start time
		m_start_time = std::chrono::system_clock::now();
	}
			
	//if 60 game minutes have passed, update hour, reset minutes
	if( m_minutes >= 60)
	{
		m_hours++;
		m_minutes = 0;
	}
			
	//if 8 game hours have passed
	if(m_hours >= 8)
	{
		//move on to next day
		m_current_day++;
		//reset to 1 if current day is 8
		if(m_current_day == 8)
		{
			m_current_day = 1;
			
			//generate life events for the new week that is starting
			WorldSystem::RandomlyGenerateLifeEvents();
		}
		
		//reset game hour
		m_hours = 0;
	}
}

std::string WorldSystem::GetDayString()
{
	std::string day = "none";
	
	switch(m_current_day)
	{
		case 1:{ day = "Monday"; break;}
		case 2:{ day = "Tuesday"; break;}
		case 3:{ day = "Wednesday"; break;}
		case 4:{ day = "Thursday"; break;}
		case 5:{ day = "Friday"; break;}
		case 6:{ day = "Saturday"; break;}
		case 7:{ day = "Sunday"; break;}
		default:{break;}
	}
	
	return day;
}
	
std::uint16_t WorldSystem::GetMinutes(){return m_minutes;}

std::uint8_t WorldSystem::GetHours(){return m_hours;}

void WorldSystem::RandomlyGenerateLifeEvents()
{
	//randomly generate life event for the week
}
