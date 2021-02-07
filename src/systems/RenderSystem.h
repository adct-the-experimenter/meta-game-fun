#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include <cstdint>
#include "core/system.h"

#include <array>
#include <vector>
#include "misc/camera.h"


class RenderSystem : public System
{
public:
	void Init(std::vector <CustomCamera> *cameras,std::uint8_t num_players);

	void Update();
	
	void UnloadTexture();
	
private:
	std::vector <CustomCamera> *m_cameras_ptr;
	
	RenderTexture2D m_target_texture;
	
	std::uint8_t m_num_players;
	
	std::vector <RenderTexture2D> viewport_textures;
	std::vector <Rectangle> m_viewports;
	void InitViewportsForThisNumberOfPlayers(std::uint8_t num_players);
};

#endif
