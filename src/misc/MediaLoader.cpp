#include "MediaLoader.h"

#include "globalvariables.h"

Texture2D title_menu_texture;

MediaLoader::MediaLoader()
{
	
}

bool MediaLoader::loadMedia()
{
	std::string filepath = DATADIR_STR + "/title_menu.png";
	title_menu_texture = LoadTexture(filepath.c_str());
	
	return true;
}

void freeMedia()
{
	UnloadTexture(title_menu_texture);
}
