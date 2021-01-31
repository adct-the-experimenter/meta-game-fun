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
	
	get_event_description = false;
	iterator_event_description = -1;
	
	//randomly generate this life events for the week
	WorldSystem::RandomlyGenerateLifeEvents();
}

void WorldSystem::Update()
{
	KeepTime();
	
	for (auto const& entity : mEntities)
	{
		//std::cout << "entity works in world system!\n";
		
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
			
			
			WorldSystem::CheckLifeEvents();
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
	for(size_t i = 0; i < life_events_week.size(); i++)
	{
		life_events_week[i].event_day = GetRandomValue(1,7);
		life_events_week[i].event_hours = GetRandomValue(2,6);
		life_events_week[i].happened = false;
	}
	
}

void WorldSystem::CheckLifeEvents()
{
	//if current time matches life event time and it has not occurred yet
	for (size_t i = 0; i < life_events_week.size(); i++)
	{
		if(m_current_day == life_events_week[i].event_day 
			&& m_hours == life_events_week[i].event_hours
			&& !life_events_week[i].happened)
		{
			//show player message about what has happened
			//to him or her 
			
			//decrease value.
		}
		else
		{
			if(m_current_day == life_events_week[i].event_day &&
			   m_hours == life_events_week[i].event_hours - 2)
			{
				get_event_description = true;
				iterator_event_description = i;
			}
		}
	}
}

void WorldSystem::handle_events(KeyboardInput& input)
{
	//if need to get an event description
	if(get_event_description)
	{
		//set text description based on input from keyboard
		
		if(input.backspace_pressed)
		{
			//delete char from descrition
			if (event_typing_box.letterCount > 0)
			{
				event_typing_box.letterCount--;
			}
			 
			event_typing_box.text[event_typing_box.letterCount] = '\0';
		}
		else if(input.enter_pressed)
		{
			//save event typing box text to life event description
			life_events_week[iterator_event_description].description = event_typing_box.text;
			
			//reset conditions for getting life event description
			get_event_description = false;
			iterator_event_description = -1;
			
		}
		else
		{
			//add char to description
			if(input.valid_key)
			{
				event_typing_box.text[event_typing_box.letterCount] = input.key;
				event_typing_box.letterCount++;
			}
			
			
			//std::cout << "name: " <<  player_char_boxes[i].typing_slots[0].text << std::endl;
		}
		
	}
}

void WorldSystem::render()
{
	if(get_event_description)
	{
		
	}
}
