#include "TileManager.h"

#include "pugixml.hpp"
#include <iostream>

#include "globalvariables.h"
#include "raylib.h"

TileMap* levelOne_map;

TileManager::TileManager()
{
	
}

bool TileManager::LoadLevel(std::uint16_t level)
{
	//load map
	
	std::string mapFilePath;
	std::string tilesheetDescriptionFilePath;
	
	TileMap* tile_map;
	
	switch(level)
	{
		case 0:
		{  
			mapFilePath = DATADIR_STR + "/suburb-level-map.xml";
			tilesheetDescriptionFilePath = DATADIR_STR + "/suburb-tilesheet-description.xml";
			tile_maps_vec.push_back(TileMap());
			tile_map = &tile_maps_vec[0];
			levelOne_map = tile_map;
			break;
		}
	}
	
	if(mapFilePath != "" && tilesheetDescriptionFilePath != "")
	{
		if( !TileManager::LoadDataFromXMLFile(mapFilePath, tilesheetDescriptionFilePath,
												*tile_map) )
		{
			std::cout << "Failed to load level" << int(level) << "!\n";
			return false;
		}
	}
	else
	{
		std::cout << "Uninitialized map file path and tilesheet description path.";
		return false;
	}
	
	return true;
}


bool TileManager::LoadDataBasedOnTilesheetDescription(std::string filepath, TileMap& tile_map)
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
    
    //add data directory
    ts_path = DATADIR_STR + "/" + ts_path;
    
    //load tile sheet
    tile_map.tilesheetTexture = LoadTexture(ts_path.c_str());
    
    //set up tile selector based on data
    pugi::xml_node tileRoot = root.child("Tiles");
    
    size_t iterator = 0;
	//go through each tile type in tiles node
	for (pugi::xml_node tile_node = tileRoot.first_child(); tile_node; tile_node = tile_node.next_sibling())
	{	
		Rectangle frame_clip;
		
		//set type
		TileType type = TileType::NONE;
		
		std::string valString = tile_node.attribute("type").value();
		//std::cout << valString << std::endl;
		
		if(valString ==  "PUSHBACK"){type = TileType::PUSH_BACK; }
		else if(valString ==  "BACKGROUND"){ type = TileType::BACKGROUND; }
		else{std::cout << "Tile type not handled!" << "i:" << iterator << std::endl; }
				
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
		
		frame_clip = {x,y,width,height};
		
		//set tile number
		valString = tile_node.attribute("num").value();
		std::uint32_t tile_number = atoi(valString.c_str());
		
		//push into frame clip map
		tile_map.frame_clip_map.push_back(frame_clip);
		
	}
	
	return true;	
}

bool TileManager::LoadDataFromXMLFile(std::string mapFilePath, std::string tilesheetDescriptionFilePath,
									TileMap& tile_map)
{
	//read tilesheet description xml file for tile parameters
	if(!TileManager::LoadDataBasedOnTilesheetDescription(tilesheetDescriptionFilePath, tile_map))
	{
		std::cout << "Failed to read tilesheet description xml!\n";
		return false;
	}
	else
	{
		// Create empty XML document within memory
		pugi::xml_document doc;
		
		// Load XML file into memory
		// Remark: to fully read declaration entries you have to specify
		// "pugi::parse_declaration"
		pugi::xml_parse_result result = doc.load_file(mapFilePath.c_str(),
													pugi::parse_default);
		if (!result)
		{
			std::cout << "File: " << mapFilePath << std::endl;
			std::cout << "Parse error: " << result.description()
				<< ", character pos= " << result.offset;
			return false;
		}
		
		pugi::xml_node root = doc.child("RootMap");
		
		//read data from level map xml file
		std::string valString;
		
		//read number of tiles
		pugi::xml_node numTilesNode = root.child("NumberOfTiles");
		
		valString = numTilesNode.attribute("num").value();
		
		tile_map.tiles.resize( size_t( atoi(valString.c_str()) ) );
		
		//read level dimensions
		pugi::xml_node dimensionsNode = root.child("Dimensions");
		
		valString = dimensionsNode.attribute("width").value();
		tile_map.levelWidth = atoi(valString.c_str());
		
		valString = dimensionsNode.attribute("height").value();
		tile_map.levelHeight = atoi(valString.c_str());
		
		//read tile dimensions
		pugi::xml_node tile_dimensions = root.child("TileDimensions");
		
		valString = tile_dimensions.attribute("width").value();
		tile_map.tileWidth = atoi(valString.c_str());
		
		valString = tile_dimensions.attribute("height").value();
		tile_map.tileHeight = atoi(valString.c_str());
		
		//for every tile in xml file
		
		pugi::xml_node tileRoot = root.child("Tiles");
		size_t iterator = 0;
		
		//go through each tile in tiles node
		for (pugi::xml_node tile_node = tileRoot.first_child(); tile_node; tile_node = tile_node.next_sibling())
		{	
			Tile tile;
			
			//set type
			TileType type = TileType::NONE;
			
			valString = tile_node.attribute("type").value();
			//std::cout << valString << std::endl;
			
			if(valString ==  "PUSHBACK"){type = TileType::PUSH_BACK; }
			else if(valString ==  "BACKGROUND"){ type = TileType::BACKGROUND; }
			else{std::cout << "Tile type not handled!" << "i:" << iterator << std::endl; }
			
			tile.type = type;
			
			//set tile number
			
			valString = tile_node.attribute("num").value();
			tile.tile_number = atoi(valString.c_str());
			
			//set frame clip pointer based on tile number
			
			tile.frame_clip_ptr = &tile_map.frame_clip_map[tile.tile_number];
			
			//assign to vector
			tile_map.tiles[iterator] = tile;
			
			iterator++;
		}
		
		
		//set vector of tiles based on level dimensions
		std::uint32_t num_tiles_horiz = tile_map.levelWidth / tile_map.tileWidth;
		std::uint32_t num_tiles_vert = tile_map.levelHeight / tile_map.tileHeight;
		
		std::uint32_t x_offset = 0;
		std::uint32_t y_offset = 0;
		
		tile_map.tiles_startx = 0;
		tile_map.tiles_starty = 0;
		
		//initialize tile position
		for(size_t i = 0; i < tile_map.tiles.size(); i++)
		{
			if(i % num_tiles_horiz == 0)
			{
				x_offset = 0;
				y_offset += tile_map.tileHeight;
			}
			
			tile_map.tiles[i].x = tile_map.tiles_startx + x_offset;
			tile_map.tiles[i].y = tile_map.tiles_starty + y_offset;
			
			x_offset += tile_map.tileWidth;
			
		}
		
	}
	
	return true;
    
}

void TileManager::FreeLevels()
{
	for(size_t i = 0; i < tile_maps_vec.size(); i++)
	{
		UnloadTexture(tile_maps_vec[i].tilesheetTexture);
	}
}
