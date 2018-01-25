#pragma once
#include "ZombieObject.h"

class NormalZombieObject : public ZombieObject
{
public:
	NormalZombieObject(PlayState* playState, int startX, int startY, int health);
	~NormalZombieObject();
	void ReachedGoal();

};

