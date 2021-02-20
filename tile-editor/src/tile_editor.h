#ifndef TILE_EDITOR_H
#define TILE_EDITOR_H

#include "raylib.h"
#include <cstdint>
#include <vector>
#include <string>

#include "../src/misc/level_maps.h"

#include "../src/core/MouseInput.h"

#include <map>

struct SelectableTile
{
	std::uint32_t tile_number;
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

struct SaveButton
{
	
	//position and dimensions of save button
	Rectangle box;
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
	
	void SetSaveXMLFile(std::string filepath);
	
	bool LoadDataFromXMLFile(std::string mapFilePath, std::string tilesheetDescriptionFilePath);
	void SaveDataToXMLFile(std::string filepath);
	
	
//Game loop
	void handleInputMouse(MouseInput& input);
	
	void logic(Rectangle& camera);
	
	void render();
	
	
private:

	//path to save file
	std::string m_save_data_file;
	
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
	
	std::uint32_t m_levelWidth; 
	std::uint32_t m_levelHeight;
	
	SaveButton m_save_buttton;
	
	//map to keep track of the tile number of a tile from tile sheet.
	std::map <std::uint32_t, Rectangle> frame_clip_map;
};

#endif
