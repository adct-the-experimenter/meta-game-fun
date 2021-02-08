#ifndef TILE_EDITOR_H
#define TILE_EDITOR_H

#include "raylib.h"
#include <cstdint>
#include <vector>
#include <string>

#include "../src/misc/level_maps.h"

struct SelectableTile
{
	Rectangle frame_clip;
	Rectangle select_box;
	TileType type;
};

struct TileBox
{
	//position of tile box
	std::uint32_t x;
	std::uint32_t y;
	
	std::vector <SelectableTile> select_tiles;
	SelectableTile* current_tile;
	size_t current_tile_index;
};

class TileEditor
{
public:
	
	TileEditor();
	~TileEditor();
	
//Set up
	bool LoadDataBasedOnTilesheetDescription(std::string filepath);

	void SetLevelDimensions(std::uint32_t tileWidth, std::uint32_t tileHeight, 
							std::uint32_t levelWidth, std::uint32_t levelHeight);
	
	void SaveDataToXMLFile(std::string filepath);
	
	
//Game loop
	void handleInputMouse();
	
	void logic();
	
	void render();
	
	
private:
	//path to tilesheet
	std::string m_tilesheet_path;
	
	//Tile sheet texture
	Texture2D m_tilesheet_texture;
	
	//box that allows selecting from tiles
	TileBox m_tile_selector;
	
	//vector of tiles
	std::vector <Tile> m_tiles_vec;
	
	//last mouse click position
	float m_mouseX,m_mouseY;
	bool m_click;
	
	std::uint32_t m_tiles_startx;
	std::uint32_t m_tiles_starty;
	
	std::uint32_t m_tile_width;
	std::uint32_t m_tile_height;
};

#endif
