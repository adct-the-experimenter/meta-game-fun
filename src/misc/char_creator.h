
#ifndef CHAR_CREATOR_H
#define CHAR_CREATOR_H

#include <cstdint>
#include <vector>
#include <array>

#include "raylib.h"
#include "core/ControllerInput.h"
#include "core/entity.h"

struct Slot
{
	Rectangle frame_clip = {0,0,30,30};
	
	size_t style_choice = 0;
	size_t color_choice = 0;
};

struct CharacterBox
{
	//everyone has the smae body and head dimensions for now
	//only hair, eyes, and clothing are different.
	Slot hair_slot;
	Slot head_slot; //for skin color change
	Slot eyes_slot;
	Slot up_cloth_slot;
	Slot low_cloth_slot;
	Slot shoe_slot;
	
	std::uint8_t current_slot = 0;
	std::uint8_t last_slot = 5;
};

class CharacterCreator
{
public:
	CharacterCreator();
	~CharacterCreator();
	
	void Init(std::vector <Entity> *entities_vec_ptr, std::uint8_t num_players);
	
	//game loop functions
	
	void handle_input(ControllerInput& input);
	
	void logic();
	
	void render();
	
	void sound();
	
private:

	//vector to hold pointers to player entities
	std::vector <Entity*> player_entities_vec;
	
	//vector to hold a character box for each player
	std::vector <CharacterBox> player_char_boxes;
	
	//array to hold color choices
	std::array <Color,8> colors;
	
};

#endif
