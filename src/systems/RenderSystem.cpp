#include "RenderSystem.h"

#include "core/system.h"

#include "core/coordinator.h"

#include <iostream>

#include <cmath>

extern Coordinator gCoordinator;

#include "misc/level_maps.h"


void RenderSystem::Init(std::vector <CustomCamera> *cameras,std::uint8_t num_players)
{
	m_cameras_ptr = cameras;
    
    RenderSystem::InitViewportsForThisNumberOfPlayers(num_players);
    
    m_num_players = num_players;
    
}

bool IsObjectInCameraView(float& posX, float& posY, Rectangle& camera_rect)
{
	//half the width and height because the camera is centered on a player.
	
	if(posX < camera_rect.x)
	{
		return false;
	}
	else if(posX > camera_rect.x + (camera_rect.width / 2))
	{
		return false;
	}
	
	if(posY < camera_rect.y)
	{
		return false;
	}
	else if(posY > camera_rect.y + (camera_rect.height / 2))
	{
		return false;
	}
	
	return true;
}

void RenderLevelMapRelativeToCamera(Texture2D* tilesheet_ptr, std::vector <Tile> *levelmap_ptr,Rectangle& camera)
{
	bool render = true;
	
	if(!tilesheet_ptr)
	{
		std::cout << "Level map tilesheet texture is uninitialized in render!\n";
		render = false;
	}
	if(!levelmap_ptr)
	{
		std::cout << "Level map pointer is uninitialized in render!\n";
		render = false;
	}
	
	if(render)
	{
		for(size_t i = 0; i < levelmap_ptr->size(); i++)
		{
			bool renderTile = false;
			
			if( (levelmap_ptr->at(i).x >= camera.x) && 
				(levelmap_ptr->at(i).x <= camera.x + camera.width) &&
				(levelmap_ptr->at(i).y >= camera.y) &&
				(levelmap_ptr->at(i).y <= camera.y + camera.height))
			{
				renderTile = true;
			}
			
			if( renderTile )
			{
				
				Vector2 pos = {levelmap_ptr->at(i).x - camera.x,levelmap_ptr->at(i).y - camera.y};
				if(levelmap_ptr->at(i).frame_clip_ptr)
				{
					DrawTextureRec(*tilesheet_ptr, 
							   *levelmap_ptr->at(i).frame_clip_ptr, 
							   pos, 
							   WHITE);
				}
			}
			
		}
	}
}

void RenderSystem::Update()
{
		
	if(this->m_cameras_ptr)
	{			
		
		//for every camera
		for(size_t i = 0; i < m_cameras_ptr->size(); i++)
		{
			//if renderable object is within camera bounds.
			
			//draw to viewport texture
			BeginTextureMode(m_viewports[i].target_texture);
				
			#ifdef TILE_EDITOR
			//render tiles
			if(levelOne_tilemap_ptr)
			{
				RenderLevelMapRelativeToCamera(levelOne_tilemap_texture_ptr,levelOne_tilemap_ptr,m_cameras_ptr->at(i).camera_rect);
			}
			#endif
			
			
			if(levelOne_map)
			{
				RenderLevelMapRelativeToCamera(&levelOne_map->tilesheetTexture,&levelOne_map->tiles,m_cameras_ptr->at(i).camera_rect);
			}
				
			
			//render texture background color
			ClearBackground(RAYWHITE);
			
			//for every entity
			for (auto const& entity : mEntities)
			{
				auto& render_comp = gCoordinator.GetComponent<RenderComponent>(entity);
				auto& transform = gCoordinator.GetComponent<Transform2D>(entity);
				
				bool renderObjectBool = IsObjectInCameraView(transform.position.x,transform.position.y,m_cameras_ptr->at(i).camera_rect);
				if(renderObjectBool)
				{
					//change render position of renderable object relative to camera
					auto& render_position = gCoordinator.GetComponent<RenderPosition>(entity);
						
					render_position.overall_position.x = transform.position.x - m_cameras_ptr->at(i).camera_rect.x;
					render_position.overall_position.y = transform.position.y - m_cameras_ptr->at(i).camera_rect.y;
					
					//render object
					//if not multi part render
					if(!render_comp.multi_part)
					{
						if(render_comp.single_render_part.texture_ptr)
						{
							//adjust render component positions according to overall position
							Vector2 pos = {render_position.overall_position.x + render_comp.single_render_part.position.x,
										  render_position.overall_position.y + render_comp.single_render_part.position.y};
							
							
							DrawTextureRec(*render_comp.single_render_part.texture_ptr, 
											render_comp.single_render_part.frame_rect, 
											pos, 
											render_comp.single_render_part.tint);
						}
					}
					else
					{
						if(render_comp.multi_render_parts_vec[0].texture_ptr)
						{
							for(int c = render_comp.multi_render_parts_vec.size() - 1; c >= 0; c--)
							{
								//render from last to first to make hair render over head
								
								//adjust render component positions according to overall position
								Vector2 pos = {render_position.overall_position.x + render_comp.multi_render_parts_vec[c].position.x,
											  render_position.overall_position.y + render_comp.multi_render_parts_vec[c].position.y};
														
								DrawTextureRec(*render_comp.multi_render_parts_vec[c].texture_ptr, 
												render_comp.multi_render_parts_vec[c].frame_rect, 
												pos, 
												render_comp.multi_render_parts_vec[c].tint);
							}
						}
					}
				}
			}
			
			EndTextureMode();
			
		}
		
	}
	
	//for every viewport	
	for(size_t it = 0; it < m_viewports.size(); it++)
	{
		
		DrawTextureRec(m_viewports.at(it).target_texture.texture, 
						m_viewports.at(it).clip, 
						m_viewports.at(it).position, 
						WHITE);
	}
	
}

void RenderSystem::UnloadTexture()
{
	for(size_t i = 0; i < m_viewports.size(); i++)
	{
		UnloadRenderTexture(m_viewports[i].target_texture);
	}
	
}

void RenderSystem::InitViewportsForThisNumberOfPlayers(std::uint8_t num_players)
{
	m_viewports.resize(num_players);
	
	std::int32_t screenWidth = GetScreenWidth();
	std::int32_t screenHeight = GetScreenHeight();
	
	
	switch(num_players)
	{
		case 1:
		{
			m_viewports[0].rect.x = 0; m_viewports[0].rect.y = 0;
			m_viewports[0].rect.width = screenWidth; m_viewports[0].rect.height = screenHeight;
			break;
		}
		case 2:
		{
			m_viewports[0].rect.x = 0; m_viewports[0].rect.y = 0;
			m_viewports[0].rect.width = screenWidth; m_viewports[0].rect.height = screenHeight / 2;
			
			m_viewports[1].rect.x = 0; m_viewports[1].rect.y = screenHeight / 2;
			m_viewports[1].rect.width = screenWidth; m_viewports[1].rect.height = screenHeight / 2;
			break;
		}
		case 3:
		{
			m_viewports[0].rect.x = 0; m_viewports[0].rect.y = 0;
			m_viewports[0].rect.width = screenWidth / 2; m_viewports[0].rect.height = screenHeight / 2;
			
			m_viewports[1].rect.x = screenWidth / 2; m_viewports[1].rect.y = 0;
			m_viewports[1].rect.width = screenWidth / 2; m_viewports[1].rect.height = screenHeight / 2;
			
			m_viewports[2].rect.x = screenWidth / 4; m_viewports[2].rect.y = screenHeight / 2;
			m_viewports[2].rect.width = screenWidth / 2; m_viewports[2].rect.height = screenHeight / 2;
			break;
		}
		case 4:
		{
			m_viewports[0].rect.x = 0; m_viewports[0].rect.y = 0;
			m_viewports[0].rect.width = screenWidth / 2; m_viewports[0].rect.height = screenHeight / 2;
			
			m_viewports[1].rect.x = screenWidth / 2; m_viewports[1].rect.y = 0;
			m_viewports[1].rect.width = screenWidth / 2; m_viewports[1].rect.height = screenHeight / 2;
			
			m_viewports[2].rect.x = 0; m_viewports[2].rect.y = screenHeight / 2;
			m_viewports[2].rect.width = screenWidth / 2; m_viewports[2].rect.height = screenHeight / 2;
			
			m_viewports[3].rect.x = screenWidth / 2; m_viewports[3].rect.y = screenHeight / 2;
			m_viewports[3].rect.width = screenWidth / 2; m_viewports[3].rect.height = screenHeight / 2;
			break;
		}
		default:{std::cout << "Unsupported number of players in viewports init!\n";}
	}
	
	for(size_t i = 0; i < m_viewports.size(); i++)
	{
		m_viewports[i].target_texture = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
		SetTextureFilter(m_viewports[i].target_texture.texture, FILTER_BILINEAR);  // Texture scale filter to use
		
		m_viewports[i].clip = {0,0, 
							(float)m_viewports.at(i).target_texture.texture.width, 
							(float)-m_viewports.at(i).target_texture.texture.height};
		
			   
		m_viewports[i].position = Vector2{m_viewports.at(i).rect.x,m_viewports.at(i).rect.y};
	}
	
}
