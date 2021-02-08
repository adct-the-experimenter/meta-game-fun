#include "tile_editor.h"

#include "pugixml.hpp"
#include <iostream>

TileEditor::TileEditor()
{
	//tile placement area
	m_tiles_startx = 250;
	m_tiles_starty = 200;
	
	m_click = false;
	
	m_tile_selector.current_tile_index = 0;
	m_tile_selector.current_tile = &m_tile_selector.select_tiles[0];
	
	m_tile_selector.x = 30;
	m_tile_selector.y = 100;
	
}

TileEditor::~TileEditor()
{
	UnloadTexture(m_tilesheet_texture);
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

bool TileEditor::LoadDataBasedOnTilesheetDescription(std::string filepath)
{
	//read xml file 
		
	// Create empty XML document within memory
    pugi::xml_document doc;
    
    // Load XML file into memory
    // Remark: to fully read declaration entries you have to specify
    // "pugi::parse_declaration"
    pugi::xml_parse_result result = doc.load_file(filepath.c_str(),
												pugi::parse_default);
    if (!result)
    {
		std::cout << "File: " << filepath << std::endl;
        std::cout << "Parse error: " << result.description()
            << ", character pos= " << result.offset;
        return false;
    }
    
    pugi::xml_node root = doc.child("TileDescriptionXML");
    
    //save path to tilesheet
    pugi::xml_node tilesheet_node = root.child("Tilesheet");
    std::string ts_path = tilesheet_node.attribute("path").value();
    
    m_tilesheet_path = ts_path;
    
    //load tile sheet
    m_tilesheet_texture = LoadTexture(m_tilesheet_path.c_str());
    
    //set up tile selector based on data
    pugi::xml_node tileRoot = root.child("Tiles");
    
    size_t iterator = 0;
	//go through each tile type in tiles node
	for (pugi::xml_node tile_node = tileRoot.first_child(); tile_node; tile_node = tile_node.next_sibling())
	{	
		SelectableTile sTile;
		
		//set type
		TileType type = TileType::NONE;
		
		std::string valString = tile_node.attribute("type").value();
		//std::cout << valString << std::endl;
		
		if(valString ==  "PUSHBACK"){type = TileType::PUSH_BACK; }
		else if(valString ==  "BACKGROUND"){ type = TileType::BACKGROUND; }
		else{std::cout << "Tile type not handled!" << "i:" << iterator << std::endl; }
		
		sTile.type = type;
		
		//set frame clip
		
		std::uint32_t x,y,width,height;
		
		valString = tile_node.attribute("x").value();
		x = atoi(valString.c_str());
		
		valString = tile_node.attribute("y").value();
		y = atoi(valString.c_str());
		
		valString = tile_node.attribute("width").value();
		width = atoi(valString.c_str());
		
		valString = tile_node.attribute("height").value();
		height = atoi(valString.c_str());
		
		sTile.frame_clip = {x,y,width,height};
		
		//push into vector
		m_tile_selector.select_tiles.push_back(sTile);
		
	}
	
	//set select box for each select tile
	
	std::uint32_t x_offset = 0;
	std::uint32_t y_offset = 0;
	
	for(size_t i = 0; i < m_tile_selector.select_tiles.size(); i++)
	{
		 
		if(i % 2)
		{
			x_offset = 0;
			y_offset += m_tile_selector.select_tiles[i].frame_clip.height;
		}
		
		m_tile_selector.select_tiles[i].select_box.x = m_tile_selector.x + x_offset;
		m_tile_selector.select_tiles[i].select_box.y = m_tile_selector.y + y_offset;
		m_tile_selector.select_tiles[i].select_box.width = m_tile_selector.select_tiles[i].frame_clip.width;
		m_tile_selector.select_tiles[i].select_box.height = m_tile_selector.select_tiles[i].frame_clip.height;
		
		x_offset += m_tile_selector.select_tiles[i].frame_clip.width;
	}
	
	
	return true;	
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
	std::cout << "Creating level map xml file...\n";
	
	// Generate new XML document within memory
    pugi::xml_document doc;
    
    // Generate XML declaration
    auto declarationNode = doc.append_child(pugi::node_declaration);
    declarationNode.append_attribute("version")    = "1.0";
    declarationNode.append_attribute("encoding")   = "ISO-8859-1";
    declarationNode.append_attribute("standalone") = "yes";
    
    // A valid XML doc must contain a single root node of any name
    auto root = doc.append_child("LevelMap");
    
    //save number of tiles in level
	std::cout << "Number of tiles: " << m_tiles_vec.size() << std::endl;
	
	//save level dimensions
    
    //create tiles node
    pugi::xml_node tilesNode = root.append_child("Tiles");
    
    //save tile type
		
	for(size_t i = 0; i < m_tiles_vec.size(); i++)
	{
		 // Add child based on tile type
		pugi::xml_node nodeChild = tilesNode.append_child("Type");
		
		switch(m_tiles_vec[i].type)
		{
			case TileType::PUSH_BACK:{ nodeChild.append_child(pugi::node_pcdata).set_value("PUSHBACK"); break;}
			case TileType::BACKGROUND:{ nodeChild.append_child(pugi::node_pcdata).set_value("BACKGROUND"); break;}
			default:{std::cout << "Tile type not handled!\n"; break;}
		}
		
	}

    
    // Save XML tree to file.
    // Remark: second optional param is indent string to be used;
    // default indentation is tab character.
	bool saveSucceeded = doc.save_file(filepath.c_str(), PUGIXML_TEXT("  "));
	
	if(saveSucceeded)
	{
		std::cout << "Error! Save was not successful!\n";
	}
	else
	{
		std::cout << "Finished saving level map xml file!\n";
	}
	
}

void TileEditor::handleInputMouse()
{
	//save last mouse click position
	
	
	m_click = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
	
	if(m_click)
	{
		m_mouseX = GetMousePosition().x;
		m_mouseY = GetMousePosition().y;
	}
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
				m_tile_selector.current_tile_index = i;
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
	for(size_t i = 0; i < m_tile_selector.select_tiles.size(); i++)
	{
		Vector2 pos = {m_tile_selector.select_tiles[i].select_box.x,
						m_tile_selector.select_tiles[i].select_box.y};
		
		//if a tile is selected render it with yellow tint
		if(i == m_tile_selector.current_tile_index)
		{
			DrawTextureRec(m_tilesheet_texture, 
						   m_tile_selector.select_tiles[i].frame_clip, 
						   pos, 
						   YELLOW);
		}
		//else render it normally
		else
		{
			
			DrawTextureRec(m_tilesheet_texture, 
						   m_tile_selector.select_tiles[i].frame_clip, 
						   pos, 
						   WHITE);
		}
	}
	
	//render tile placement area
	for(size_t i = 0; i < m_tiles_vec.size(); i++)
	{
		Vector2 pos = {m_tiles_vec[i].x,m_tiles_vec[i].y};
		
		if(m_tiles_vec[i].frame_clip_ptr)
		{
			DrawTextureRec(m_tilesheet_texture, 
					   *m_tiles_vec[i].frame_clip_ptr, 
					   pos, 
					   WHITE);
		}
		
	}
}
