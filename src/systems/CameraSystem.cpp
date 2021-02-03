#include "CameraSystem.h"

#include "core/system.h"

#include "core/coordinator.h"

#include <iostream>

extern Coordinator gCoordinator;


void CameraSystem::Init(std::array <CustomCamera,4> *cameras, std::uint8_t num_players,
				std::uint16_t screenWidth, std::uint16_t screenHeight)
{
	m_cameras_ptr = cameras;
	
	m_num_players = num_players;
	
	//Initialize camera size based on number of players
	for(size_t i = 0; i < num_players; i++)
	{
		m_cameras_ptr->at(i).num_player = i + 1;
		
		m_cameras_ptr->at(i).camera.width = (screenWidth / 2);
		m_cameras_ptr->at(i).camera.height = (screenHeight / 2);
	}
	
	//initialize position based on number of players
	switch(num_players)
	{
		case 1:
		{
			m_cameras_ptr->at(0).camera.x = 0;
			m_cameras_ptr->at(0).camera.y = 0;
			break;
		}
		case 2:
		{
			m_cameras_ptr->at(0).camera.x = 0;
			m_cameras_ptr->at(0).camera.y = 0;
			
			m_cameras_ptr->at(1).camera.x = screenWidth / 2;
			m_cameras_ptr->at(1).camera.y = 0;
		}
		default:{break;}
	}
}

void CameraSystem::Update()
{
	for (auto const& entity : mEntities)
	{
		auto& transform = gCoordinator.GetComponent<Transform2D>(entity);
		auto& render_position = gCoordinator.GetComponent<RenderPosition>(entity);
		
		for(size_t i = 0; i < m_num_players; i++)
		{
			render_position.overall_position.x = transform.position.x - m_cameras_ptr->at(i).camera.x;
			render_position.overall_position.y = transform.position.y - m_cameras_ptr->at(i).camera.y;
		}
		
	}
	
}

