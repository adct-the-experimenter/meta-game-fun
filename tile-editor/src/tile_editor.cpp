#include "tile_editor.h"

TileEditor::TileEditor()
{
	
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

void TileEditor::SetLevelDimensions(std::uint32_t tile_width, std::uint32_t tileHeight, 
							std::uint32_t levelWidth, std::uint32_t levelHeight)
{
	//set vector of tiles based on 
	
}

void TileEditor::SaveDataToXMLFile(std::string filepath)
{
	//remove empty tiles from vector
}

void TileEditor::handleInputMouse()
{
	//save last mouse click position
}
	
void TileEditor::logic()
{
	//if tile box tile is clicked on 
	//for loop here
	//set current tile to the one that was clicked on. use index of vector to set pointer
	
	//if area for placing tiles was clicked on
	//for loop here
		//set tile type in array
}
	
void TileEditor::render()
{
	//render tile box 
	
		//if a tile is selected render it with yellow tint
		//else render it normally
	
	//render tile placement area
}
