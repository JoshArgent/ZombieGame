#pragma once
#include "DisplayableObject.h"

class PlayState;;
class Coordinate;

class TileObject : public DisplayableObject
{
public:
	TileObject(PlayState* playState, int tileX, int tileY);
	~TileObject();
	void setTilePosition(int tileX, int tileY);
	Coordinate getTilePosition();
protected:
	PlayState *playState;
};
