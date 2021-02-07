#ifndef TILE_EDITOR_H
#define TILE_EDITOR_H

#include <cstdint>
#include <vector>

enum class TileType : std::uint8_t {NONE=0, PUSH_BACK, BACKGROUND}

struct Tile
{
	std::uint32_t x;
	std::uint32_t y;
	TileType type;
};

struct SelectableTile
{
	Rectangle frame_clip;
	Rectangle select_box;
	TileType type;
};

struct TileBox
{
	std::vector <SelectableTile> select_tiles;
	SelectableTile* current_tile;
};

class TileEditor
{
public:
	
	TileEditor();
	~TileEditor();
	
//Set up
	void LoadDataBasedOnTilesheetDescription(std::string filepath);

	void SetLevelDimensions(std::uint32_t tile_width, std::uint32_t tileHeight, 
							std::uint32_t levelWidth, std::uint32_t levelHeight);
	
	void SaveDataToXMLFile(std::string filepath);
	
//Game loop
	void handleInputMouse();
	
	void logic();
	
	void render();
	
	
private:
	//path to tilesheet
	
	//path to xml file describing tile sheet
	
	//box that allows selecting from tiles
	TileBox m_tile_selector;
	
	//vector of tiles
	std::vector <Tile> m_tiles_vec;
	
	//last mouse click position
	float m_mouseX,m_mouseY;
};

#endif
