#include "char_creator.h"


#include "core/coordinator.h"

#include "misc/media.h" //for texture

extern Coordinator gCoordinator;

CharacterCreator::CharacterCreator()
{
	std::array <Color,8> t_colors = {WHITE,BLUE,BEIGE,BLACK,BROWN,RED,GOLD,LIGHTGRAY};
	colors = t_colors;
	
	move_next_state = false;
}

CharacterCreator::~CharacterCreator()
{
	
}

void CharacterCreator::Init(std::vector <Entity> *entities_vec_ptr, std::uint8_t num_players)
{
	//initialize vector holding pointers to player entitites
	//assuming first entities in entire entity vector are player entities.
	
	player_entities_vec.resize(num_players);
	
	for(size_t i = 0; i < player_entities_vec.size(); i++)
	{
		player_entities_vec[i] = &entities_vec_ptr->at(i);
	}
	
	player_char_boxes.resize(num_players);
	
	char_confirmations.resize(num_players);
}

void CharacterCreator::handle_input(ControllerInput& input)
{
	
	//if joystick moved up, go up a slot
	if(input.gamepad_p1.y_axis < -32600)
	{
		if(player_char_boxes[0].current_slot > 0){player_char_boxes[0].current_slot--;}
	}
	//else if joystick moved down, go down a slot
	else if(input.gamepad_p1.y_axis > 32600)
	{
		if(player_char_boxes[0].current_slot < player_char_boxes[0].last_slot){player_char_boxes[0].current_slot++;}
	}
		
	//if joystick moved left, go left on color choice
	if(input.gamepad_p1.x_axis < -32600)
	{
		Slot* slot_ptr = &player_char_boxes[0].slots[player_char_boxes[0].current_slot];
		
		if(slot_ptr->color_choice > 0){slot_ptr->color_choice--;}
	}
	//if joystick moved right, go right on color choice
	else if(input.gamepad_p1.x_axis > 32600)
	{
		Slot* slot_ptr = &player_char_boxes[0].slots[player_char_boxes[0].current_slot];
		
		if(slot_ptr->color_choice < 7){slot_ptr->color_choice++;}
	}
	
	//if a button pressed, turn confirm bool on
	if(input.gamepad_p1.button == SDL_CONTROLLER_BUTTON_A)
	{
		player_char_boxes[0].confirm_selection = true;
	}
}
	
void CharacterCreator::logic()
{
	int width = 30;
	
	for(size_t i = 0; i < player_char_boxes.size(); i++)
	{
		//set frame clip based on style choice slot
		for(size_t slot_it = 0; slot_it < player_char_boxes[i].slots.size(); slot_it++)
		{
			player_char_boxes[i].slots[slot_it].frame_clip = {30*player_char_boxes[i].slots[slot_it].style_choice,slot_it*30,width,width};
		}
		
		//if selection confirmed
		if(player_char_boxes[i].confirm_selection)
		{
			//create new render components for player entity, and set player info based on choices.
			//if not already created
			if(!char_confirmations[i])
			{
				char_confirmations[i] = true;
				
				Texture2D* texture_player = &rpg_sprite_sheet_texture;
				gCoordinator.AddComponent(
							*player_entities_vec[i],
							RenderInfo{
								.texture_ptr = texture_player,
								.frame_rect = player_char_boxes[i].slots[1].frame_clip,
								.tint = colors[ player_char_boxes[i].slots[1].color_choice ],
								.part_description = RenderPartDescription::HEAD
							}
						);
				
			}
			
		}
		
	}
	
	//check if all players confirmed character creations
	bool all_confirmed = true;
	for(size_t i = 0; i < char_confirmations.size(); i++)
	{
		all_confirmed = char_confirmations[i];
		if(!all_confirmed){break;}
	}
	
	if(all_confirmed)
	{
		move_next_state = true;
	}
	
}

void CharacterCreator::render()
{
	//render body part slots
	for(size_t i = 0; i < player_char_boxes.size(); i++)
	{
		Vector2 position = {100*(i+1),0};
		
		//draw textures of each slot
		
		for(size_t slot_it = 0; slot_it < player_char_boxes[i].slots.size(); slot_it++)
		{
			position.y = (slot_it + 1)*50;
			
			DrawTextureRec(rpg_sprite_sheet_texture,
						player_char_boxes[i].slots[slot_it].frame_clip,
						position,
						colors[player_char_boxes[i].slots[slot_it].color_choice]
						);
		}
		
	}
	
}

void CharacterCreator::sound()
{
	//play some character select music
}

bool CharacterCreator::MoveToNextStateBool(){return move_next_state;}
