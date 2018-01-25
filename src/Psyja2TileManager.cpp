#include "Psyja2TileManager.h"
#include "SpriteManager.h"
#include "Psyja2Engine.h"
#include <sstream>

Psyja2TileManager::Psyja2TileManager(void) : TileManager(50, 50)
{

}

Psyja2TileManager::~Psyja2TileManager()
{

}

void Psyja2TileManager::DrawTileAt(BaseEngine* pEngine, SDL_Surface* pSurface, int iMapX, int iMapY, int iStartPositionScreenX, int iStartPositionScreenY) const
{
	unsigned int color = 0xffffff;

	int n = GetValue(iMapX, iMapY);
	if (n >= 0 && n < 13)
	{
		ImageData *tile = ((Psyja2Engine*)pEngine)->spriteManager->GetTiles()[n];
		tile->RenderImage(pSurface, 0, 0, iStartPositionScreenX, iStartPositionScreenY, tile->GetWidth(), tile->GetHeight());
		tile->RenderImage(pSurface, 0, 0, iStartPositionScreenX + 25, iStartPositionScreenY, tile->GetWidth(), tile->GetHeight());
		tile->RenderImage(pSurface, 0, 0, iStartPositionScreenX, iStartPositionScreenY + 25, tile->GetWidth(), tile->GetHeight());
		tile->RenderImage(pSurface, 0, 0, iStartPositionScreenX + 25, iStartPositionScreenY + 25, tile->GetWidth(), tile->GetHeight());
	}
	// Debug: draw tile coordinate
	/*std::ostringstream ss;
	std::string str;
	ss << std::to_string(iMapX) << ", " << std::to_string(iMapY);
	pEngine->DrawString(iStartPositionScreenX, iStartPositionScreenY, ss.str().c_str(), 0xFF0000, NULL, pSurface);*/
}
