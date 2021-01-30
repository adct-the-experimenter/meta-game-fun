#include "char_creator.h"


#include "core/coordinator.h"

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
	//for now only color is changed, styles come in later
	
	//if joystick moved up, go up a slot
	
	//else if joystick moved down, go down a slot
	
	//if joystick moved left, go left on color choice
	
	//if joystick moved right, go right on color choice
	
	//if a button pressed, turn confirm bool on
	
}
	
void CharacterCreator::logic()
{
	
	//set frame clip based on slot
	
	//if on slot ok and confirm bool is on, 
	//create new render components for player, and set player info based on choices.
	
}

void CharacterCreator::render()
{
	//render body part slots
	for(size_t i = 0; i < player_char_boxes.size(); i++)
	{
		Vector2 position = {20*i,50};
		
		//draw textures of each slot
		
		
		DrawTextureRec(player_char_boxes[i].hair_slot.sprite_sheet,player_char_boxes[i].hair_slot.frame_clip,
						position,colors[player_char_boxes[i].hair_slot.color_choice]
						);
	
		position.y = 2*50;
		
		DrawTextureRec(player_char_boxes[i].head_slot.sprite_sheet,player_char_boxes[i].head_slot.frame_clip,
						position,colors[player_char_boxes[i].head_slot.color_choice]
						);
		
		position.y = 3*50;
		
		DrawTextureRec(player_char_boxes[i].eyes_slot.sprite_sheet,player_char_boxes[i].eyes_slot.frame_clip,
						position,colors[player_char_boxes[i].eyes_slot.color_choice]
						);
		
		position.y = 4*50;
		
		DrawTextureRec(player_char_boxes[i].up_cloth_slot.sprite_sheet,player_char_boxes[i].up_cloth_slot.frame_clip,
						position,colors[player_char_boxes[i].up_cloth_slot.color_choice]
						);
		
		position.y = 5*50;
		
		DrawTextureRec(player_char_boxes[i].low_cloth_slot.sprite_sheet,player_char_boxes[i].low_cloth_slot.frame_clip,
						position,colors[player_char_boxes[i].low_cloth_slot.color_choice]
						);
						
		position.y = 6*50;
		DrawTextureRec(player_char_boxes[i].shoe_slot.sprite_sheet,player_char_boxes[i].shoe_slot.frame_clip,
						position,colors[player_char_boxes[i].shoe_slot.color_choice]
						);
		
	}
	
}

void CharacterCreator::sound()
{
	//play some character select music
}
