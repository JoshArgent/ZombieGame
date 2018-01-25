#pragma once
#include "DisplayableObject.h"

class PlayState;

class BulletObject : public DisplayableObject
{
public:
	BulletObject(PlayState* playState, double startX, double startY, double direction);
	~BulletObject();
	void Draw();
	void DoUpdate(int iCurrentTime);
protected:
	PlayState* playState;
private:
	double xInc;
	double yInc;
	double x;
	double y;
};

