#pragma once
#include "TileObject.h"
#include "Coordinate.h"
#include "Psyja2Engine.h"
#include "PlayState.h"

TileObject::TileObject(PlayState* playState, int tileX, int tileY) : DisplayableObject(playState->engine), playState(playState)
{
	m_iPreviousScreenX = 0;
	m_iPreviousScreenY = 0;
	setTilePosition(tileX, tileY);
}

TileObject::~TileObject()
{
}

void TileObject::setTilePosition(int tileX, int tileY)
{
	m_iCurrentScreenX = tileX * 50 + 25 - (m_iDrawWidth / 2);
	m_iCurrentScreenY = tileY * 50 + 75 - (m_iDrawHeight / 2);
}

Coordinate TileObject::getTilePosition()
{
	return Coordinate(GetXCentre() / 50, GetYCentre() / 50 - 1);
}
