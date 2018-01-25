#pragma once
#include "TileObject.h"

class PlayerObject;

class PlayerObject : public TileObject
{
public:
	PlayerObject(PlayState* playState);
	~PlayerObject();
private:
	void DetectCollisions(int orgX, int orgY, double orgAngle, int& x, int& y, double& angle);
	double angle = 0;
	const double MAX_SPEED = 3;
	const double ACCELERATION = 0.1;
	double velocityX = 0;
	double velocityY = 0;
	double health = 1;
	unsigned int skinColor = 0xF5B2B6;
	unsigned int clothesColor = 0x008DD2;
	unsigned int hairColor = 0x86776F;
public:
	void Draw();
	void SetAngle(double angle);
	void DoUpdate(int iCurrentTime);
	void Damage(double amount);
	double GetHealth();
	void SetHealth(double health);
	int score = 0;
	int lives = 3;
};
