#ifndef LEVEL_MAP_H
#define LEVEL_MAP_H

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

#endif
