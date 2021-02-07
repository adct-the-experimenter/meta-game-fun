#include "tile_editor.h"

#include "pugixml.hpp"

TileEditor::TileEditor()
{
	//tile placement area
	m_tiles_startx = 250;
	m_tiles_starty = 200;
	
	m_click = false;
}

TileEditor::~TileEditor()
{
	
}

bool MouseInBox(float mouseX, float mouseY, Rectangle box)	
{
	if( mouseX < box.x)
	{
		return false;
	}
	else if(mouseX > box.x + box.width)
	{
		return false;
	}
	
	if(mouseY < box.y)
	{
		return false;
	}
	else if(mouseY > box.y + box.height)
	{
		return false;
	}
	
	return true;
}

void TileEditor::LoadDataBasedOnTilesheetDescription(std::string filepath)
{
	//read xml file 
	
		//save path to tilesheet
	
		//set up tile selector based on data
		
		
}

void TileEditor::SetLevelDimensions(std::uint32_t tileWidth, std::uint32_t tileHeight, 
							std::uint32_t levelWidth, std::uint32_t levelHeight)
{
	//set vector of tiles based on level dimensions
	std::uint32_t num_tiles_horiz = levelWidth / tileWidth;
	std::uint32_t num_tiles_vert = levelHeight / tileHeight;
	
	std::uint32_t num_tiles_total = num_tiles_horiz * num_tiles_vert;
	
	m_tiles_vec.resize(num_tiles_total);
	
	std::uint32_t x_offset = 0;
	std::uint32_t y_offset = 0;
	
	//intiialize tile 
	for(size_t i = 0; i < m_tiles_vec.size(); i++)
	{
		if(i == num_tiles_horiz)
		{
			x_offset = 0;
			y_offset += tileHeight;
		}
		
		m_tiles_vec[i].x = m_tiles_startx + x_offset;
		m_tiles_vec[i].y = m_tiles_starty + y_offset;
		m_tiles_vec[i].frame_clip_ptr = nullptr;
		
		x_offset += tileWidth;
	}
	
	m_tile_width = tileWidth;
	m_tile_height = tileHeight;
}

void TileEditor::SaveDataToXMLFile(std::string filepath)
{
	//save number of tiles in level
	
	//save level dimensions
	
	//save tile type
}

void TileEditor::handleInputMouse()
{
	//save last mouse click position
}
	
void TileEditor::logic()
{
	if(m_click)
	{
		//if tile box tile is clicked on 
		//for loop here
		
		for(size_t i = 0; i < m_tile_selector.select_tiles.size(); i++)
		{
			Rectangle box = {m_tiles_vec[i].x,m_tiles_vec[i].y,m_tile_width,m_tile_height};
			//set tile type in array
			if(MouseInBox(m_mouseX,m_mouseY,box))
			{
				//set current tile to the one that was clicked on. use index of vector to set pointer
				m_tile_selector.current_tile = &m_tile_selector.select_tiles[i];
			}
			
		}
		
		//if area for placing tiles was clicked on
		//for loop here
		for(size_t i = 0;i < m_tiles_vec.size(); i++)
		{
			Rectangle box = {m_tiles_vec[i].x,m_tiles_vec[i].y,m_tile_width,m_tile_height};
			//set tile type in array
			if(MouseInBox(m_mouseX,m_mouseY,box))
			{
				if(m_tile_selector.current_tile)
				{
					m_tiles_vec[i].frame_clip_ptr = &m_tile_selector.current_tile->frame_clip;
					m_tiles_vec[i].type = m_tile_selector.current_tile->type;
				}
				
			}
		}
			
	}
	
}
	
void TileEditor::render()
{
	//render tile box 
	
		//if a tile is selected render it with yellow tint
		//else render it normally
	
	//render tile placement area
}
