
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
	
	//everyone has the same body and head dimensions for now
	//only hair, eyes, and clothing are different.
	
	//hair = 0, head=1,eyes=2,upper clothing = 3, lower clothing = 4, shoes=5
	std::array <Slot,6> slots;
	
	std::uint8_t current_slot = 0;
	std::uint8_t last_slot = 5;
	bool confirm_selection = false;
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
	
	bool MoveToNextStateBool();
	
private:

	//vector to hold pointers to player entities
	std::vector <Entity*> player_entities_vec;
	
	//vector to hold a character box for each player
	std::vector <CharacterBox> player_char_boxes;
	
	//array to hold color choices
	std::array <Color,8> colors;
	
	//vector to hold bools for character creation confirmation for players
	std::vector <bool> char_confirmations;
	
	bool move_next_state;
};

#endif
