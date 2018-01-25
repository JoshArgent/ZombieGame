#include "HelperFunctions.h"
#include "Coordinate.h"
#include <list>

#pragma once

class Psyja2TileManager;

class Map
{
public:
	Map(Psyja2TileManager *tilemanager, int** mapGrid, int width, int height);
	Map(Psyja2TileManager *tilemanager, const char *filename);
	~Map();
private:
	int** mapGrid;
	int width;
	int height;
	Psyja2TileManager *tilemanager;
public:
	void LoadMap(const char *filename);
	// Uses A* algorithm to calculate the next x,y coordinates for a path to the goalX and goalY
	// Returns an ordered list of coordinates as a path to the goal
	std::list<Coordinate> calculatePath(const int x, const int y, const int goalX, const int goalY) const;
	// Returns the type of tile at this x,y coord
	int Get(const int x, const int y) const;
	void Set(const int x, const int y, const int val);
	bool IsValidCoordinate(const int x, const int y) const;
	bool IsObstacle(const int x, const int y) const;
	bool IsMud(const int x, const int y) const;
	bool IsPointCollision(const int x, const int y);
	bool IsPolygonCollision(const int num, const double xps[], const double yps[]);
	void SpreadMud();
	int GetWidth();
	int GetHeight();
	int spawnX = 1;
	int spawnY = 1;
private:
	void ReloadTileManager();
};

