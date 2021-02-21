#include "PhysicsSystem.h"

#include "core/system.h"

#include "core/coordinator.h"

#include "misc/level_maps.h"
#include <iostream>
#include <cmath> //for rounding

extern Coordinator gCoordinator;


void PhysicsSystem::Init()
{
	
}

void PushBack(float& posX, float& posY, float& velX, float& velY, float& dt)
{
    float pushBackDist = 2.0f;
    
    float newPosX, newPosY;
    //if going right, push left
    if(velX > 0){newPosX = posX - pushBackDist - (velX * dt);}
    //else if going left, push right
    else if(velX < 0){newPosX = posX + pushBackDist - (velX * dt);}
    else if(velX == 0){newPosX = posX + pushBackDist;}
    
	posX = newPosX;
    
    //if going down, push up
    if(velY > 0){newPosY = posY - pushBackDist - (velY * dt);}
    //else if going up, push down
    else if(velY < 0){newPosY = posY + pushBackDist - (velY * dt);}
    else if(velY == 0){newPosY = posY + pushBackDist; }

    
    posY = newPosY;
}

bool CollisionWithTileDetected(std::uint32_t& tile_x, std::uint32_t& tile_y, std::uint32_t tile_width,
						   float& obj_x, float& obj_y, std::uint32_t obj_width, std::uint32_t& obj_height)
{
	//assuming object has width and height of 30 and it is centered
	
	float objLeftX = obj_x;
	float objRightX = obj_x + obj_width;
	float objTopY = obj_y;
	float objBottomY = obj_y + obj_height;
	
	std::uint32_t tileLeftX = tile_x;
	std::uint32_t tileRightX = tile_x + tile_width;
	std::uint32_t tileTopY = tile_y;
	std::uint32_t tileBottomY = tile_y + tile_width;
	
	//for collision to be true, all conditions must be true. AABB square collsion detection, all
	//The left edge x-position of [A] must be less than the right edge x-position of [B].
    //The right edge x-position of [A] must be greater than the left edge x-position of [B].
    //The top edge y-position of [A] must be less than the bottom edge y-position of [B].
    //The bottom edge y-position of [A] must be greater than the top edge y-position of [B].
    
    if(objBottomY <= tileTopY)
	{
		return false;
	}
	
	if(objTopY >= tileBottomY)
	{
		return false;
	}
    
    if(objRightX <= tileLeftX)
	{
		return false;
	}
	
	if(objLeftX >= tileRightX)
	{
		return false;
	}
	
	return true;
}

void PhysicsSystem::Update(float& dt)
{
	
	for (auto const& entity : mEntities)
	{
		auto& rigidBody = gCoordinator.GetComponent<RigidBody2D>(entity);
		auto& transform = gCoordinator.GetComponent<Transform2D>(entity);
		auto& physics_type_comp = gCoordinator.GetComponent<PhysicsTypeComponent>(entity);
		auto& collisionBox = gCoordinator.GetComponent<CollisionBox>(entity);
		
		// Forces
		auto const& gravity = gCoordinator.GetComponent<Gravity2D>(entity);
		
		switch(physics_type_comp.phy_type)
		{
			case PhysicsType::LIFE_RPG:
			{
				//move transform component by velocity of rigid body multiplied by time
				transform.position.x += rigidBody.velocity.x * dt;
				transform.position.y += rigidBody.velocity.y * dt;
				
				
				PhysicsSystem::CheckCollisionWithTilesRPG(transform.position.x, transform.position.y,
														  rigidBody.velocity.x, rigidBody.velocity.y,
														  dt,
														  collisionBox.width, collisionBox.height);
				
				break;
			}
			case PhysicsType::PLATFORMER:
			{
				//account for acceleration due to gravity to rigid body velocity
				rigidBody.velocity.x += gravity.force.x * dt;
				rigidBody.velocity.y += gravity.force.y * dt;
				
				//move transform component by velocity of rigid body multiplied by time
				transform.position.x += rigidBody.velocity.x * dt;
				transform.position.y += rigidBody.velocity.y * dt;
				
				break;
			}
			default:{break;}
		}
		
	}
	
}

void PhysicsSystem::CheckCollisionWithTilesRPG(float& obj_x, float& obj_y, 
												float& obj_vx, float& obj_vy, 
												float& dt, 
												std::uint32_t& obj_width,
												std::uint32_t& obj_height)
{
	#ifndef TILE_EDITOR
	if(levelOne_map)
	{
		//get start index for tiles close to player,
		//done to skip tiles not close to player
		std::uint32_t col = obj_x * levelOne_map->tileWidth / levelOne_map->levelWidth;
		std::uint32_t row = obj_y * levelOne_map->tileHeight / levelOne_map->levelHeight;
		
		size_t player_index = 0; 
		
		player_index = col + trunc(row * (levelOne_map->levelHeight / levelOne_map->tileHeight));
		//std::cout << "player index:" << player_index << std::endl;
		
		//set start index relative to player
		size_t start_index = player_index / 2;
		
		if(start_index >= levelOne_map->tiles.size()){start_index = levelOne_map->tiles.size() - 20;}
		
		//only check for 3 rows of tiles below player index
		size_t end_index = player_index + 3*(levelOne_map->levelWidth / levelOne_map->tileWidth);
		
		if(end_index >= levelOne_map->tiles.size()){end_index = levelOne_map->tiles.size() - 1;}
		//std::cout << "start index: " << start_index << std::endl;
		//std::cout << "end index: " << end_index << std::endl;
		
		
		//start_index = 0;
		//end_index = levelOne_map->tiles.size() - 1;
		for(size_t i = start_index; i < end_index; i++)
		{
			Tile* tile =  &levelOne_map->tiles[i];
			if(tile->type == TileType::PUSH_BACK)
			{
				
				if(CollisionWithTileDetected(tile->x, tile->y, levelOne_map->tileWidth,
						obj_x, obj_y, 
						obj_width, obj_height)
					)
				{
					
					//std::cout << "Collision detected for tile " << i << " !\n";
					//push back player 
					PushBack(obj_x, obj_y, 
							obj_vx, obj_vy, 
							dt);
				}
			}
		}
	}
	#endif
}
