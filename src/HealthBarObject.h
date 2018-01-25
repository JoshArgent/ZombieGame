#pragma once
#include "DisplayableObject.h"

class PlayState;

class HealthBarObject : public DisplayableObject
{
public:
	HealthBarObject(PlayState* playState);
	~HealthBarObject();
	void Draw();
private:
	PlayState* playState;
	const int BAR_WIDTH = 200;
	const int BAR_HEIGHT = 20;
};

