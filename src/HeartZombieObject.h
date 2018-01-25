#pragma once
#include "ZombieObject.h"

class HeartZombieObject : public ZombieObject
{
public:
	HeartZombieObject(PlayState* playState, int startX, int startY, int health);
	~HeartZombieObject();
	void ReachedGoal();
	void DoZombieUpdate();
	std::vector<ZombieColor> GetColorOptions();
};

