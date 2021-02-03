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
	
	show_event = false;
	
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
			
			
			WorldSystem::CheckLifeEvents(player);
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
		//update game minutes by 1
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
		//set time
		life_events_week[i].event_day = GetRandomValue(1,7);
		life_events_week[i].event_hours = GetRandomValue(2,6);
		life_events_week[i].happened = false;
		
		//set status
		life_events_week[i].effect = GetRandomValue(-1,1); //-1 loss, 0 nothing, +1 gain
		life_events_week[i].status_affected = StatusAffected(GetRandomValue(0,2));
		life_events_week[i].num_player_affected = 1 /*GetRandomValue(1,4)*/;
		
		life_events_week[i].description_entered = false;
		 
	}
	 /*test event quickly
	life_events_week[0].event_day = 1;
	life_events_week[0].event_hours = 1;
	 */
}

void WorldSystem::CheckLifeEvents(Player& player)
{
	//if current time matches life event time and it has not occurred yet
	for (size_t i = 0; i < life_events_week.size(); i++)
	{
		//if player being operated on is the one that the event happens to
		if(player.num_player == life_events_week[i].num_player_affected)
		{
			if(m_current_day == life_events_week[i].event_day 
			&& m_hours == life_events_week[i].event_hours
			&& !life_events_week[i].happened)
			{
				//show player message about what has happened
				//to him or her 
				show_event = true;
				iterator_event_description = i;
				
				life_events_week[i].happened = true;
				
				life_events_week[i].name_of_affected = player.name;
				
				//assuming first entities in array are players
				
				//apply effect from life event to player
				switch(life_events_week[i].status_affected)
				{
					case StatusAffected::WEALTH:
					{
						if(life_events_week[i].effect < -1 && player.money <= 0)
						{
							player.money = 0;
						}
						else
						{
							player.money += (life_events_week[i].effect * 20);
						}
						break;
					}
					case StatusAffected::HEALTH:
					{
						if(life_events_week[i].effect < -1 && player.health <= 0)
						{
							player.health = 0;
						}
						else
						{
							player.health += (life_events_week[i].effect * 20);
						}
						
						break;
					}
					case StatusAffected::LOOKS:
					{
						
						if(player.look_status != LooksStatus::SUPER_UGLY && 
							life_events_week[i].effect < -1)
						{
							
						}
						else
						{
							int value = int(player.look_status) + life_events_week[i].effect;
							if(!value){value = 0;}
							player.look_status = LooksStatus(value);
						}
						
						break;
					}
				}
			}
			else
			{
				if(!life_events_week[i].description_entered && !life_events_week[i].happened)
				{
					if(m_current_day == life_events_week[i].event_day &&
						m_hours == life_events_week[i].event_hours - 1)
					{
						WorldSystem::SetGetEventDescriptionBool(true);
						iterator_event_description = i;
					}
				}
				
			}
		}
		
	}
}

void WorldSystem::handle_events(KeyboardInput& input)
{
	//if need to get an event description
	if(WorldSystem::GetGetEventDescriptionBool())
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
		
		if(input.enter_pressed)
		{
			//save event typing box text to life event description
			life_events_week[iterator_event_description].description = event_typing_box.text;
			life_events_week[iterator_event_description].description_entered = true;
			
			//reset conditions for getting life event description
			WorldSystem::SetGetEventDescriptionBool(false);
			iterator_event_description = -1;
			
			//clear textbox text
			char *begin = event_typing_box.text;
			char *end = begin + sizeof(event_typing_box.text);
			std::fill(begin, end, 0);
			
		}
		else
		{
			//add char to description
			if(input.valid_key)
			{
				event_typing_box.text[event_typing_box.letterCount] = input.key;
				event_typing_box.letterCount++;
			}
			
		}
		
	}
	
	if(show_event)
	{
		if(input.enter_pressed)
		{
			show_event = false;
			iterator_event_description = -1;
		}
	}
}

void WorldSystem::render()
{
	//render time info from world system at top
	std::string time_info = "Day: " + WorldSystem::GetDayString() + "  " \
							"Hour: " + std::to_string(WorldSystem::GetHours()) + "  " \
							"Minute: " + std::to_string(WorldSystem::GetMinutes());
							
	DrawText(time_info.c_str(), 190, 20, 20, LIGHTGRAY);
	
	if(WorldSystem::GetGetEventDescriptionBool() || show_event)
	{
		Rectangle rec = {200, 200, 200, 300};
		
		//draw rectangle background
		DrawRectangleRec(rec, LIGHTGRAY);
		
		std::string stat;
		std::string effect;
		if(iterator_event_description != -1)
		{
			switch(life_events_week[iterator_event_description].status_affected)
			{
				case StatusAffected::WEALTH:{stat = "wealth"; break;}
				case StatusAffected::HEALTH:{stat = "health"; break;}
				case StatusAffected::LOOKS:{stat = "looks"; break;}
			}
			
			if(life_events_week[iterator_event_description].effect <= -1)
			{
				effect = "-" + std::to_string(-1 * life_events_week[iterator_event_description].effect);
			}
			else if(life_events_week[iterator_event_description].effect == 0)
			{
				effect = "0" ;
			}
			else if(life_events_week[iterator_event_description].effect >= 1)
			{
				effect = "+" + std::to_string(life_events_week[iterator_event_description].effect);
			}
		}
		
		
		std::string status_str = " Status Affected: " + stat;
		
		std::string effect_str = " Effect: " + effect;
		
		std::string full_text;
		if(WorldSystem::GetGetEventDescriptionBool())
		{
			full_text = status_str + "\n" + effect_str + "\n" + " Enter what happened below: 100 character limit\n\n "
								+ std::string(event_typing_box.text) + "\n";
		}
		else if(show_event)
		{
			std::string affected = " Affected Player: " + life_events_week[iterator_event_description].name_of_affected;
			
			full_text = affected + "\n" + status_str + "\n" + effect_str + "\n" + " Description:\n\n "
						+ life_events_week[iterator_event_description].description + "\n";
		}
		
		Font font = GetFontDefault();
		
		DrawTextRec(font,full_text.c_str(),rec,12,2.0f, true, RAYWHITE); //word wrap is true
	}

}

void WorldSystem::SetGetEventDescriptionBool(bool state){get_event_description = state;}

bool WorldSystem::GetGetEventDescriptionBool(){return get_event_description;}
