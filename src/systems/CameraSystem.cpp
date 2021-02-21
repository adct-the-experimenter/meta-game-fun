#include "CameraSystem.h"

#include "core/system.h"

#include "core/coordinator.h"

#include <iostream>

extern Coordinator gCoordinator;


void CameraSystem::Init(std::vector <CustomCamera> *cameras, std::uint8_t num_players,
				std::uint16_t screenWidth, std::uint16_t screenHeight)
{
	m_cameras_ptr = cameras;
	
	m_num_players = num_players;
	
	//Initialize camera size based on number of players
	for(size_t i = 0; i < num_players; i++)
	{
		m_cameras_ptr->at(i).num_player = i + 1;
		
		m_cameras_ptr->at(i).camera_rect.width = (screenWidth / num_players);
		m_cameras_ptr->at(i).camera_rect.height = (screenHeight / num_players);
	}
	
	
}

void CameraSystem::Update()
{
	std::uint8_t current_player = 0;
	
	for (auto const& entity : mEntities)
	{
		
		auto& transform = gCoordinator.GetComponent<Transform2D>(entity);
		auto& player = gCoordinator.GetComponent<Player>(entity);
		
		
		//center camera position with player
		//15 is half of the width of the player character
		//45 is half of the player's width
		
		m_cameras_ptr->at(player.num_player - 1).camera_rect.x = (transform.position.x + 15) - (m_cameras_ptr->at(player.num_player - 1).camera_rect.width / 2);
		m_cameras_ptr->at(player.num_player - 1).camera_rect.y = (transform.position.y + 45) - (m_cameras_ptr->at(player.num_player - 1).camera_rect.height / 2);
		
		
		//bounds check
		if(m_cameras_ptr->at(player.num_player - 1).camera_rect.x < 0)
		{
			m_cameras_ptr->at(player.num_player - 1).camera_rect.x = 0;
		}
		
		if(m_cameras_ptr->at(player.num_player - 1).camera_rect.y < 0)
		{
			m_cameras_ptr->at(player.num_player - 1).camera_rect.y = 0;
		}
		
		/*
		std::cout << "Player " << int(player.num_player) << " , Camera(x,y): " << m_cameras_ptr->at(player.num_player - 1).camera_rect.x << "," 
		<< m_cameras_ptr->at(player.num_player - 1).camera_rect.y << " Player transform(x,y): " << 
		transform.position.x << "," << transform.position.y << std::endl;
		*/
		
	}
	
}

