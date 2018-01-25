#pragma once
#include "TileObject.h"
#include "Coordinate.h"
#include "HelperFunctions.h"
#include "Polygon.h"
#include <math.h>
#include <list>

struct ZombieColor
{
	ZombieColor::ZombieColor(unsigned int skin, unsigned int clothes) : skin(skin), clothes(clothes) { }
	ZombieColor::ZombieColor() { }
	unsigned int skin;
	unsigned int clothes;
};

class ZombieObject : public TileObject
{
public:
	ZombieObject(PlayState* playState, int startX, int startY, int health);
	~ZombieObject();
	void Draw();
	void DoUpdate(int iCurrentTime);
	std::vector<Polygon> GetBounds();
private:
	// movement parameters
	const int width = 30;
	const int height = 30;
	const double SPEED = 1.5;
	double xOff = 0;
	double yOff = 0;
	double angle = 0;
	unsigned int skinColor = 0;
	unsigned int clothesColor = 0;

protected:
	TileObject *goalObject;
	Coordinate goalTileCache = Coordinate(0, 0);
	std::list<Coordinate> pathCache;
	int health;

public:
	// Sets the target object for the zombie to track
	void SetGoal(TileObject *goalObject);

	// Called when the zombie enters a new tile
	virtual void MovedTile(int tileX, int tileY);

	// Called when the zombie reaches it's goal object
	virtual void ReachedGoal();

	// Returns a vector list of possible zombie color schemes
	virtual std::vector<ZombieColor> GetColorOptions();

	// Called BEFORE a game onUpdate is called on the zombie
	virtual void DoZombieUpdate();
	
	// Returns the health of the zombie
	int GetHealth();

	// Sets the health of the zombie
	void SetHealth(int health);
};
