#include "char_creator.h"


#include "core/coordinator.h"

#include "misc/media.h" //for texture

extern Coordinator gCoordinator;

CharacterCreator::CharacterCreator()
{
	std::array <Color,8> t_colors = {WHITE,BEIGE,BLACK,DARKBROWN,BROWN,RED,GOLD,LIGHTGRAY};
	colors = t_colors;
	
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
	
	
}

void CharacterCreator::handle_input(ControllerInput& input)
{
	
	//if joystick moved up, go up a slot
	if(input.gamepad_p1.y_axis > 0)
	{
		if(player_char_boxes[0].current_slot > 0){player_char_boxes[0].current_slot--;}
	}
	//else if joystick moved down, go down a slot
	else if(input.gamepad_p1.y_axis < 0)
	{
		if(player_char_boxes[0].current_slot < player_char_boxes[0].last_slot){player_char_boxes[0].current_slot++;}
	}
		
	//if joystick moved left, go left on color choice
	if(input.gamepad_p1.x_axis > 0)
	{
		Slot* slot_ptr;
		switch(player_char_boxes[0].current_slot)
		{
			case 0:{ slot_ptr = &player_char_boxes[0].hair_slot; break;}
			case 1:{ slot_ptr = &player_char_boxes[0].head_slot; break;}
			case 2:{ slot_ptr = &player_char_boxes[0].eyes_slot; break;}
			case 3:{ slot_ptr = &player_char_boxes[0].up_cloth_slot; break;}
			case 4:{ slot_ptr = &player_char_boxes[0].low_cloth_slot; break;}
			case 5:{ slot_ptr = &player_char_boxes[0].shoe_slot; break;}
		}
		
		if(slot_ptr->color_choice > 0){slot_ptr->color_choice--;}
	}
	//if joystick moved right, go right on color choice
	else if(input.gamepad_p1.x_axis < 0)
	{
		Slot* slot_ptr;
		switch(player_char_boxes[0].current_slot)
		{
			case 0:{ slot_ptr = &player_char_boxes[0].hair_slot; break;}
			case 1:{ slot_ptr = &player_char_boxes[0].head_slot; break;}
			case 2:{ slot_ptr = &player_char_boxes[0].eyes_slot; break;}
			case 3:{ slot_ptr = &player_char_boxes[0].up_cloth_slot; break;}
			case 4:{ slot_ptr = &player_char_boxes[0].low_cloth_slot; break;}
			case 5:{ slot_ptr = &player_char_boxes[0].shoe_slot; break;}
		}
		
		if(slot_ptr->color_choice < 7){slot_ptr->color_choice++;}
	}
	
	//if a button pressed, turn confirm bool on
	
}
	
void CharacterCreator::logic()
{
	int width = 30;
	
	//set frame clip based on style choice slot
	for(size_t i = 0; i < player_char_boxes.size(); i++)
	{
		player_char_boxes[i].hair_slot.frame_clip = {30*player_char_boxes[i].hair_slot.style_choice,0,width,width};
		player_char_boxes[i].head_slot.frame_clip = {30*player_char_boxes[i].head_slot.style_choice,30,width,width};
		player_char_boxes[i].eyes_slot.frame_clip = {30*player_char_boxes[i].eyes_slot.style_choice,60,width,width};
		player_char_boxes[i].up_cloth_slot.frame_clip = {30*player_char_boxes[i].up_cloth_slot.style_choice,90,width,width};
		player_char_boxes[i].low_cloth_slot.frame_clip = {30*player_char_boxes[i].low_cloth_slot.style_choice,120,width,width};
		player_char_boxes[i].shoe_slot.frame_clip = {30*player_char_boxes[i].shoe_slot.style_choice,150,width,width};
		
	}
	//if on slot ok and confirm bool is on, 
	//create new render components for player, and set player info based on choices.
	
}

void CharacterCreator::render()
{
	//render body part slots
	for(size_t i = 0; i < player_char_boxes.size(); i++)
	{
		Vector2 position = {100*(i+1),50};
		
		//draw textures of each slot
		
		
		DrawTextureRec(rpg_sprite_sheet_texture,player_char_boxes[i].hair_slot.frame_clip,
						position,colors[player_char_boxes[i].hair_slot.color_choice]
						);
	
		position.y = 2*50;
		
		DrawTextureRec(rpg_sprite_sheet_texture,player_char_boxes[i].head_slot.frame_clip,
						position,colors[player_char_boxes[i].head_slot.color_choice]
						);
		
		position.y = 3*50;
		
		DrawTextureRec(rpg_sprite_sheet_texture,player_char_boxes[i].eyes_slot.frame_clip,
						position,colors[player_char_boxes[i].eyes_slot.color_choice]
						);
		
		position.y = 4*50;
		
		DrawTextureRec(rpg_sprite_sheet_texture,player_char_boxes[i].up_cloth_slot.frame_clip,
						position,colors[player_char_boxes[i].up_cloth_slot.color_choice]
						);
		
		position.y = 5*50;
		
		DrawTextureRec(rpg_sprite_sheet_texture,player_char_boxes[i].low_cloth_slot.frame_clip,
						position,colors[player_char_boxes[i].low_cloth_slot.color_choice]
						);
						
		position.y = 6*50;
		DrawTextureRec(rpg_sprite_sheet_texture,player_char_boxes[i].shoe_slot.frame_clip,
						position,colors[player_char_boxes[i].shoe_slot.color_choice]
						);
		
	}
	
}

void CharacterCreator::sound()
{
	//play some character select music
}
