#pragma once
#include "ZombieObject.h"

class CoinZombieObject : public ZombieObject
{
public:
	CoinZombieObject(PlayState* playState, int startX, int startY, int health);
	~CoinZombieObject();
	void ReachedGoal();
	void MovedTile(int tileX, int tileY);
	std::vector<ZombieColor> GetColorOptions();
};

