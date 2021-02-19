#ifndef TILE_MANAGER_H
#define TILE_MANAGER_H

#include "level_maps.h"
#include <vector>


class TileManager
{
public:
	
	TileManager();
	
	bool LoadLevel(std::uint16_t level);
	
	bool LoadDataBasedOnTilesheetDescription(std::string filepath, TileMap& tile_map);
	
	bool LoadDataFromXMLFile(std::string mapFilePath, std::string tilesheetDescriptionFilePath,
									TileMap& thisTileMap);
	
	void FreeLevels();
	
private:
	
	std::vector <TileMap> tile_maps_vec;
	
	
	
};

#endif
