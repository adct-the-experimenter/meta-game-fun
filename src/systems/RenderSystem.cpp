#include "RenderSystem.h"

#include "core/system.h"

#include "core/coordinator.h"

#include <iostream>

#include <cmath>

extern Coordinator gCoordinator;

#define max(a, b) ((a)>(b)? (a) : (b))
#define min(a, b) ((a)<(b)? (a) : (b))


void RenderSystem::Init(std::vector <CustomCamera> *cameras,std::uint8_t num_players)
{
	m_cameras_ptr = cameras;
    
    RenderSystem::InitViewportsForThisNumberOfPlayers(num_players);
    
    m_num_players = num_players;
    
}

bool IsObjectInCameraView(float& posX, float& posY, Rectangle& camera_rect)
{
	
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

void RenderSystem::Update()
{
	
		
	if(this->m_cameras_ptr)
	{			
		
		//for every camera
		for(size_t i = 0; i < m_cameras_ptr->size(); i++)
		{
			//if renderable object is within camera bounds.
			
				BeginTextureMode(viewport_textures[i]);
				
				//render texture background color
				ClearBackground(RAYWHITE);
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
								for(size_t c = render_comp.multi_render_parts_vec.size(); c >= 1; c--)
								{
									//render from last to first to make hair render over head
									
									//adjust render component positions according to overall position
									Vector2 pos = {render_position.overall_position.x + render_comp.multi_render_parts_vec[c-1].position.x,
												  render_position.overall_position.y + render_comp.multi_render_parts_vec[c-1].position.y};
															
									DrawTextureRec(*render_comp.multi_render_parts_vec[c-1].texture_ptr, 
													render_comp.multi_render_parts_vec[c-1].frame_rect, 
													pos, 
													render_comp.multi_render_parts_vec[c-1].tint);
								}
							}
						}
					}
				}
			
			EndTextureMode();
			
		}
		
	}
		
	
	
	for(size_t it = 0; it < viewport_textures.size(); it++)
	{
		Rectangle source = {0.0f, 0.0f, (float)viewport_textures[it].texture.width, (float)-viewport_textures[it].texture.height};
					Rectangle dest = {(float)m_viewports.at(it).x, m_viewports.at(it).y,
						   (float)m_cameras_ptr->at(it).camera_rect.width, (float)m_cameras_ptr->at(it).camera_rect.height };
		DrawTexturePro(viewport_textures[it].texture, source, dest, (Vector2){ 0, 0 }, 0.0f, WHITE);
	}
}

void RenderSystem::UnloadTexture()
{
	for(size_t i = 0; i < viewport_textures.size(); i++)
	{
		UnloadRenderTexture(viewport_textures[i]);
	}
	
}

void RenderSystem::InitViewportsForThisNumberOfPlayers(std::uint8_t num_players)
{
	m_viewports.resize(num_players);
	
	std::int32_t screenWidth = GetScreenWidth();
	std::int32_t screenHeight = GetScreenHeight();
	
	viewport_textures.resize(num_players);
	
	for(size_t i = 0; i < viewport_textures.size(); i++)
	{
		viewport_textures[i] = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
		SetTextureFilter(viewport_textures[i].texture, FILTER_BILINEAR);  // Texture scale filter to use
	}
	
	switch(num_players)
	{
		case 1:
		{
			m_viewports[0].x = 0; m_viewports[0].y = 0;
			m_viewports[0].width = screenWidth; m_viewports[0].height = screenHeight;
			break;
		}
		case 2:
		{
			m_viewports[0].x = 0; m_viewports[0].y = 0;
			m_viewports[0].width = screenWidth; m_viewports[0].height = screenHeight / 2;
			
			m_viewports[1].x = 0; m_viewports[1].y = screenHeight / 2;
			m_viewports[1].width = screenWidth / 2; m_viewports[1].height = screenHeight / 2;
			break;
		}
		case 3:
		{
			m_viewports[0].x = 0; m_viewports[0].y = 0;
			m_viewports[0].width = screenWidth; m_viewports[0].height = screenHeight;
			
			m_viewports[1].x = screenWidth / 2; m_viewports[1].y = 0;
			m_viewports[1].width = screenWidth / 2; m_viewports[1].height = screenHeight / 2;
			
			m_viewports[2].x = screenWidth / 4; m_viewports[2].y = screenHeight / 2;
			m_viewports[2].width = screenWidth / 2; m_viewports[2].height = screenHeight / 2;
			break;
		}
		case 4:
		{
			m_viewports[0].x = 0; m_viewports[0].y = 0;
			m_viewports[0].width = screenWidth; m_viewports[0].height = screenHeight;
			
			m_viewports[1].x = screenWidth / 2; m_viewports[1].y = 0;
			m_viewports[1].width = screenWidth / 2; m_viewports[1].height = screenHeight / 2;
			
			m_viewports[2].x = 0; m_viewports[2].y = screenHeight / 2;
			m_viewports[2].width = screenWidth / 2; m_viewports[2].height = screenHeight / 2;
			
			m_viewports[3].x = screenWidth / 2; m_viewports[3].y = screenHeight / 2;
			m_viewports[3].width = screenWidth / 2; m_viewports[3].height = screenHeight / 2;
			break;
		}
		default:{std::cout << "Unsupported number of players in viewports init!\n";}
	}
}
