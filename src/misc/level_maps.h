#ifndef LEVEL_MAP_H
#define LEVEL_MAP_H

#include "raylib.h"
#include <vector>
#include <cstdint>
#include <iostream>


enum class TileType : std::uint8_t {NONE=0, PUSH_BACK, BACKGROUND};

struct Tile
{
	std::uint32_t x;
	std::uint32_t y;
	TileType type;
	Rectangle* frame_clip_ptr;
	std::uint32_t tile_number;
};

extern Texture2D* levelOne_tilemap_texture_ptr;
extern std::vector <Tile> *levelOne_tilemap_ptr;
extern std::uint32_t levelOne_tilewidth;
extern std::uint32_t levelOne_tileheight;

struct TileMap
{
	std::vector <Tile> tiles;
	std::vector <Rectangle> frame_clip_map;

	std::uint32_t levelWidth;
	std::uint32_t levelHeight;
	std::uint32_t tiles_startx;
	std::uint32_t tiles_starty;
	
	std::uint32_t tileWidth;
	std::uint32_t tileHeight;
	Texture2D tilesheetTexture;
	
};

extern TileMap* levelOne_map;

#endif
