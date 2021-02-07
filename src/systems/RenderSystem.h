#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include <cstdint>
#include "core/system.h"

#include <array>
#include <vector>
#include "misc/camera.h"

struct RenderViewport
{
	Rectangle rect;
	RenderTexture2D target_texture;
};

class RenderSystem : public System
{
public:
	void Init(std::vector <CustomCamera> *cameras,std::uint8_t num_players);

	void Update();
	
	void UnloadTexture();
	
private:
	std::vector <CustomCamera> *m_cameras_ptr;
		
	std::uint8_t m_num_players;
	
	std::vector <RenderViewport> m_viewports;
	void InitViewportsForThisNumberOfPlayers(std::uint8_t num_players);
};

#endif
