#include "char_creator.h"

#include "core/system.h"
#include "core/coordinator.h"

#include "misc/media.h" //for texture

extern Coordinator gCoordinator;

CharacterCreator::CharacterCreator()
{
	std::array <Color,8> t_colors = {WHITE,BLUE,BEIGE,GRAY,BROWN,RED,GOLD,LIGHTGRAY};
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
	
	//initialize typing slot text box locations
	for(size_t it = 0; it < player_char_boxes.size(); it++)
	{
		player_char_boxes[it].typing_slots[0].textBox = {100*(it+1),20,50,20};
		player_char_boxes[it].typing_slots[1].textBox = {100*(it+1),40,50,20};
	}
}

void CharacterCreator::handle_input(ControllerInput& controller_input, KeyboardInput& key_input)
{
	CharacterCreator::handle_controller_input(controller_input);
	CharacterCreator::handle_keyboard_input(key_input);
}

const int16_t joystick_border = 32600;

void CharacterCreator::handle_controller_input(ControllerInput& input)
{
	//number of character boxes and number of players should be the same
	
	//for each controller
	for(size_t i = 0; i < input.gamepads_vec.size();i++)
	{
		//if joystick moved up, go up a slot
		if(input.gamepads_vec[i].y_dir_axis < -joystick_border)
		{
			if(player_char_boxes[i].current_slot > 0){player_char_boxes[i].current_slot--;}
		}
		//else if joystick moved down, go down a slot
		else if(input.gamepads_vec[i].y_dir_axis > joystick_border)
		{
			if(player_char_boxes[i].current_slot < last_slot){player_char_boxes[i].current_slot++;}
		}
			
		//if joystick moved left, go left on color choice
		if(input.gamepads_vec[i].x_dir_axis < -joystick_border)
		{
			if(player_char_boxes[i].current_slot >= render_slot)
			{
				RenderSlot* slot_ptr = &player_char_boxes[i].render_slots[player_char_boxes[i].current_slot - 2];
			
				if(slot_ptr->color_choice > 0){slot_ptr->color_choice--;}
			}
			
		}
		//if joystick moved right, go right on color choice
		else if(input.gamepads_vec[i].x_dir_axis > joystick_border)
		{
			if(player_char_boxes[i].current_slot >= render_slot)
			{
				RenderSlot* slot_ptr = &player_char_boxes[i].render_slots[player_char_boxes[i].current_slot - 2];
			
				if(slot_ptr->color_choice < 7){slot_ptr->color_choice++;}
			}
			
		}
		
		//if a button pressed, turn confirm bool on
		if(input.gamepads_vec[i].button == SDL_CONTROLLER_BUTTON_A)
		{
			player_char_boxes[i].confirm_selection = true;
		}
	}
	
}

void CharacterCreator::handle_keyboard_input(KeyboardInput& input)
{
	for(size_t i = 0; i < player_char_boxes.size(); i++)
	{
		//if current slot is the name slot 
		if(player_char_boxes[i].current_slot == name_slot)
		{
			if(input.backspace_pressed)
			{
				if (player_char_boxes[i].typing_slots[0].letterCount > 0)
				{
					player_char_boxes[i].typing_slots[0].letterCount--;
				}
				 
                player_char_boxes[i].typing_slots[0].text[player_char_boxes[i].typing_slots[0].letterCount] = '\0';
			}
			else if(input.enter_pressed)
			{
				//do nothing
			}
			else
			{
				if(input.valid_key)
				{
					player_char_boxes[i].typing_slots[0].text[player_char_boxes[i].typing_slots[0].letterCount] = input.key;
					player_char_boxes[i].typing_slots[0].letterCount++;
				}
                
                
                //std::cout << "name: " <<  player_char_boxes[i].typing_slots[0].text << std::endl;
			}
			
		}
		//if current slot is the job slot
		if(player_char_boxes[i].current_slot == job_slot)
		{
			if(input.backspace_pressed)
			{
				if (player_char_boxes[i].typing_slots[1].letterCount > 0)
				{
					player_char_boxes[i].typing_slots[1].letterCount--;
				}
				 
                player_char_boxes[i].typing_slots[1].text[player_char_boxes[i].typing_slots[1].letterCount] = '\0';
			}
			else if(input.enter_pressed)
			{
				//do nothing
			}
			else
			{
				if(input.valid_key)
				{
					player_char_boxes[i].typing_slots[1].text[player_char_boxes[i].typing_slots[1].letterCount] = input.key;
					player_char_boxes[i].typing_slots[1].letterCount++;
				}
			}
		}
	}
	
	
}

void CharacterCreator::logic()
{
	int width = 30;
	
	for(size_t i = 0; i < player_char_boxes.size(); i++)
	{  	

		//set frame clip based on style choice slot
		for(size_t slot_it = 0; slot_it < player_char_boxes[i].render_slots.size(); slot_it++)
		{
			player_char_boxes[i].render_slots[slot_it].frame_clip = {30*player_char_boxes[i].render_slots[slot_it].style_choice,
																	 slot_it*30,width,width};
		}
		
		
		//if selection confirmed
		if(player_char_boxes[i].confirm_selection)
		{
			//create new render components for player entity, and set player info based on choices.
			//if not already created
			if(!char_confirmations[i])
			{
				char_confirmations[i] = true;
				
				//add multiple render component 
				
				Texture2D* texture_player = &rpg_sprite_sheet_texture;
				
				std::array <RenderPartDescription,6> descript_array = {RenderPartDescription::HAIR,
																		RenderPartDescription::EYE,
																		RenderPartDescription::HEAD,
																		RenderPartDescription::UPPER_BODY_CLOTHING,
																		RenderPartDescription::LOWER_BODY_CLOTHING,
																		RenderPartDescription::SHOES};
				std::vector <RenderInfo> temp_body_parts;
				temp_body_parts.resize(6);
				
				for(size_t slot_it = 0; slot_it < 6; slot_it++)
				{
					temp_body_parts[slot_it].texture_ptr = texture_player;
					temp_body_parts[slot_it].frame_rect = player_char_boxes[i].render_slots[slot_it].frame_clip;
					temp_body_parts[slot_it].tint = colors[ player_char_boxes[i].render_slots[slot_it].color_choice ];
					temp_body_parts[slot_it].part_description = descript_array[slot_it];
					temp_body_parts[slot_it].position = {2.0f,2.0f};
				}
				
				temp_body_parts[0].position = {5.0f,10.0f};
				temp_body_parts[1].position = {5.0f,15.0f};
				temp_body_parts[2].position = {5.0f,17.0f};
				temp_body_parts[3].position = {5.0f,40.0f};
				temp_body_parts[4].position = {5.0f,65.0f};
				temp_body_parts[5].position = {5.0f,75.0f};
				
				
				
				gCoordinator.AddComponent(
								*player_entities_vec[i],
								RenderComponent{
									.multi_render_parts_vec = temp_body_parts,
									.multi_part = true
								}
							);
				
				Vector2 char_position = {40,40};
				
				//add render position
				gCoordinator.AddComponent(
								*player_entities_vec[i],
								RenderPosition{
									.overall_position = char_position
								}
							);
				
				
				//add player info component
				
				std::string name = std::string(player_char_boxes[i].typing_slots[0].text);
				std::string partner_name = "none";
				std::uint16_t balance = 200;
				std::uint8_t hp = 100;
				std::string job = std::string(player_char_boxes[i].typing_slots[1].text);
				LooksStatus look = LooksStatus::NORMAL;
				PlayerTimeStatus time_stat = PlayerTimeStatus::NONE;
				ActivityStatus activity_stat = ActivityStatus::ROAMING_WORLD;
				gCoordinator.AddComponent(*player_entities_vec.at(i), Player{.name = name, .romantic_partner_name = partner_name,
														.time_status=time_stat,
														.money = balance,
														.health = hp,
														.job_occupation = job,
														.look_status = look,
														.activity_status = activity_stat,
														.num_player = i + 1} );
														
				//add input react component
				InputReact react;
				react.actor_type = InputReactorType::PLAYER;
				react.reactToInput = true;
				react.player_num = i + 1;
								 
				gCoordinator.AddComponent(
								*player_entities_vec[i],
								react
								);
				
				//add transform
				Vector2 initP = {10.0f,60.0f};
				gCoordinator.AddComponent(
							*player_entities_vec.at(i),
							Transform2D{
								.position = initP
							}
						);
				
				//add rigid body
				Vector2 initV = {0.0f,0.0f};
				gCoordinator.AddComponent(
							*player_entities_vec.at(i),
							RigidBody2D{
								.velocity = initV
							}
						);
						
				//add gravity component for later use
				Vector2 grav = {0.0f,0.0f};
				gCoordinator.AddComponent(
							*player_entities_vec.at(i),
							Gravity2D{
								.force = grav 
							}
						);
						
				//add physics type
				PhysicsType pType = PhysicsType::LIFE_RPG;
				gCoordinator.AddComponent(
							*player_entities_vec.at(i),
							PhysicsTypeComponent{
								.phy_type = pType 
							}
						);
						
				//add collision box
				std::uint32_t width = 30;
				std::uint32_t height = 60;
				
				gCoordinator.AddComponent(
										*player_entities_vec.at(i),
										CollisionBox{
											.width = width,
											.height = height
										});
				
				//add animation component
				AnimationInfo aInfo;
				aInfo.anim_actor_type = AnimatedActorType::PLAYER;
				aInfo.horiz_frame_offset = 0;
				aInfo.frame_size = 30;
				aInfo.frame_count = 0;
				gCoordinator.AddComponent(
							*player_entities_vec.at(i),
							Animation{
								.info = aInfo
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

std::array <std::string,6> str_attr_array = {"Hair","Head", "Eyes","Upper Body", "Lower Body", "Shoes"};
void CharacterCreator::render()
{
	
	//render body part slots
	for(size_t i = 0; i < player_char_boxes.size(); i++)
	{
		Vector2 position = {100*(i+1) + i*50,0};
		
		//if selection is not confirmed
		if(!player_char_boxes[i].confirm_selection)
		{
			//draw textures of each slot
			for(size_t slot_it = 0; slot_it < player_char_boxes[i].render_slots.size(); slot_it++)
			{
				position.y = (slot_it + render_slot + 1)*50;
				
				DrawText(str_attr_array[slot_it].c_str(),position.x - 70, 
						position.y + 8, 
						12, BLACK);
						
				DrawTextureRec(rpg_sprite_sheet_texture,
							player_char_boxes[i].render_slots[slot_it].frame_clip,
							position,
							colors[player_char_boxes[i].render_slots[slot_it].color_choice]
							);
			}
			
			//render text box for typing slot
			for(size_t it = 0; it < player_char_boxes[i].typing_slots.size(); it++)
			{
				//set textbox rectangle position
				player_char_boxes[it].typing_slots[0].textBox = {100*(it+1) + 50*it,50,100,20};
				player_char_boxes[it].typing_slots[1].textBox = {100*(it+1) + 50*it,100,100,20};
				
				DrawRectangleRec(player_char_boxes[i].typing_slots[it].textBox, LIGHTGRAY);
				
				if(it == 0)
				{
					DrawText("Name:",player_char_boxes[i].typing_slots[it].textBox.x - 30, 
						player_char_boxes[i].typing_slots[it].textBox.y + 8, 
						12, BLACK);
				}
				else if(it == 1)
				{
					DrawText("Job:",player_char_boxes[i].typing_slots[it].textBox.x - 30, 
						player_char_boxes[i].typing_slots[it].textBox.y + 8, 
						12, BLACK);
				}
				
				DrawText(player_char_boxes[i].typing_slots[it].text, 
						player_char_boxes[i].typing_slots[it].textBox.x + 5, 
						player_char_boxes[i].typing_slots[it].textBox.y + 8, 
						12, MAROON);
			}
		}
		
	}
	
}

void CharacterCreator::sound()
{
	//play some character select music
}

bool CharacterCreator::MoveToNextStateBool(){return move_next_state;}
