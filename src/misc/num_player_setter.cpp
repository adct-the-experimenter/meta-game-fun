#include "num_player_setter.h"

#include "core/system.h"
#include "core/coordinator.h"


extern Coordinator gCoordinator;

NumPlayerSetter::NumPlayerSetter()
{
	
	move_next_state = false;
	
	m_current_slot = 0;
	
	m_confirm_selection = false;
}

NumPlayerSetter::~NumPlayerSetter()
{
	
}

void NumPlayerSetter::Init()
{
	
	//initialize render slot locations
	for(size_t it = 0; it < num_players_boxes.size(); it++)
	{
		num_players_boxes[it].render_rect = {400 - 25,30*(it+1) + 400,100,20};
		if(it == 0)
		{
			num_players_boxes[it].text = " " + std::to_string( it + 1) + " Player";
		}
		else
		{
			num_players_boxes[it].text = " " + std::to_string( it + 1) + " Players";
		}
	}
	
	m_font = GetFontDefault();
}

void NumPlayerSetter::handle_input(ControllerInput& controller_input, KeyboardInput& key_input)
{
	NumPlayerSetter::handle_controller_input(controller_input);
	//NumPlayerSetter::handle_keyboard_input(key_input);
}

const int16_t joystick_border = 32600;

void NumPlayerSetter::handle_controller_input(ControllerInput& input)
{
	//number of character boxes and number of players should be the same
	
	//only first game pad decides number of players
	
	size_t i = 0;
	
	//if joystick moved up, go up a slot
	if(input.gamepads_vec[i].y_dir_axis < -joystick_border)
	{
		if(m_current_slot > 0){m_current_slot--;}
	}
	//else if joystick moved down, go down a slot
	else if(input.gamepads_vec[i].y_dir_axis > joystick_border)
	{
		if(m_current_slot < 3){m_current_slot++;}
	}
		
	
	//if a button pressed, turn confirm bool on
	if(input.gamepads_vec[i].button == SDL_CONTROLLER_BUTTON_A)
	{
		m_confirm_selection = true;
	}
	
}

void NumPlayerSetter::handle_keyboard_input(KeyboardInput& input)
{	
	
}

void NumPlayerSetter::CreatePlayerEntities(std::vector <Entity> *entities_vec_ptr)
{
	//
	for(size_t i = 0; i < m_num_players; i++)
	{
		entities_vec_ptr->at(i) = gCoordinator.CreateEntity();
	}
}

void NumPlayerSetter::logic()
{	
	//if selection confirmed
	if(m_confirm_selection)
	{
		m_num_players = m_current_slot + 1;
		move_next_state = true;
	}
	
}

void NumPlayerSetter::render()
{
	
	//render body part slots
	for(size_t i = 0; i < num_players_boxes.size(); i++)
	{
		
		//draw each slot
		if(i != m_current_slot)
		{
			DrawRectangleRec(num_players_boxes[i].render_rect,WHITE);
			DrawTextRec(m_font,num_players_boxes[i].text.c_str(),num_players_boxes[i].render_rect,12,2.0f, true, BLACK);
		}
		else
		{
			DrawRectangleRec(num_players_boxes[i].render_rect,YELLOW);
			DrawTextRec(m_font,num_players_boxes[i].text.c_str(),num_players_boxes[i].render_rect,12,2.0f, true, BLACK);
		}
		
	}
	
}

void NumPlayerSetter::sound()
{
	//play some character select music
}

bool NumPlayerSetter::MoveToNextStateBool(){return move_next_state;}

std::int8_t NumPlayerSetter::GetNumberOfPlayers(){return m_num_players;}
