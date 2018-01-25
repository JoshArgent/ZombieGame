#pragma once
#include "TileManager.h"
#include <vector>

class Psyja2TileManager :
	public TileManager
{
public:
	Psyja2TileManager();
	~Psyja2TileManager();
	void DrawTileAt(
		BaseEngine* pEngine,
		SDL_Surface* pSurface,
		int iMapX, int iMapY,
		int iStartPositionScreenX, int iStartPositionScreenY) const;
};

