#include "Map.h"
#include "Psyja2TileManager.h"
#include <stdexcept>
#include <queue>
#include <list>
#include <map>
#include <fstream>
#include <string>
#include "HelperFunctions.h"

struct Move
{
	int x;
	int y;
	double g;
	double h;
	Move(int x, int y, double g, double h) : x(x), y(y), g(g), h(h) {}
};

Map::Map(Psyja2TileManager *tilemanager, int** mapGrid, int width, int height) : mapGrid(mapGrid), width(width), height(height), tilemanager(tilemanager)
{
}

Map::Map(Psyja2TileManager *tilemanager, const char *filename) : tilemanager(tilemanager)
{
	width = 16;
	height = 11;
	LoadMap(filename);
}

Map::~Map()
{
}

void Map::LoadMap(const char *filename)
{
	// Load the file
	std::ifstream file(filename);
	std::string line;
	// Define the map array
	int **map = new int*[11];
	for (int i = 0; i < 11; ++i)
		map[i] = new int[16];
	// Load the spawn point
	std::getline(file, line);
	std::string sx = line.substr(0, line.find(','));
	std::string sy = line.substr(line.find(',') + 1);
	spawnX = atoi(sx.c_str());
	spawnY = atoi(sy.c_str());
	// Read and parse each line
	int y = 0;
	while (std::getline(file, line))
	{
		int x = 0;
		std::vector<std::string> tokens = splitString(line, ' ');
		for (std::string token : tokens)
		{
			int val = atoi(token.c_str());
			map[y][x] = val;
			x++;
		}
		y++;
	}
	mapGrid = map;
	// Reset the tiles in the tile manager
	ReloadTileManager();
}

std::list<Coordinate> Map::calculatePath(const int x, const int y, const int goalX, const int goalY) const
{
	std::list<Move*> closedList;
	std::list<Move*> openList;
	std::map<Move*,Move*> cameFrom;
	openList.push_front(new Move(x, y, 0, getDistanceBetweenPoints(x, y, goalX, goalY)));
	Move *current = NULL;

	while (!openList.empty())
	{
		// Find min f value on open list
		current = NULL;
		for (std::list<Move*>::iterator it = openList.begin(); it != openList.end(); ++it)
		{
			Move *n = dynamic_cast<Move*>(*it);
			if (current == NULL || current->g + current->h > n->g + n->h)
			{
				current = n;
			}
		}
		openList.remove(current);
		closedList.push_back(current);

		// See if goal has been reached
		if (current->x == goalX && current->y == goalY)
			break;

		int cx = current->x;
		int cy = current->y;
		int ch = current->h;
		int cg = current->g;

		// Analyse neighbouring nodes
		for (int dx = -1; dx <= 1; dx++)
		for (int dy = -1; dy <= 1; dy++)
		{
			int tx = cx + dx;
			int ty = cy + dy;	
			if (IsValidCoordinate(tx, ty) && !(dx == 0 && dy == 0))
			{
				if (!IsObstacle(tx, ty))
				{
					// Only accept diagonals if the adjacent squares are clear - avoids cutting corners near obstacles
					if (abs(dx) == 1 && abs(dy) == 1)
					{
						if (!(IsValidCoordinate(tx, cy) && !IsObstacle(tx, cy) && IsValidCoordinate(cx, ty) && !IsObstacle(cx, ty)))
						{
							continue;
						}
					}

					double gInc = 1;
					if (abs(dx) == 1 && abs(dy) == 1)
						gInc = 1.4; // give higher cost for diagonal moves
					double g = cg + gInc;
					double h = getDistanceBetweenPoints(tx, ty, goalX, goalY);
					double f = g + h;
					Move *succ = new Move(tx, ty, g, h);
					
					Move* inClosed = NULL;
					for (std::list<Move*>::iterator it = closedList.begin(); it != closedList.end(); ++it)
					{
						Move *n = dynamic_cast<Move*>(*it);
						if (n->x == succ->x && n->y == succ->y)
						{
							inClosed = n;
							break;
						}
					}
					if (inClosed != NULL) // IF succ IN ClosedList
					{
						delete succ;
						continue;
					}

					Move* inOpen = NULL;
					for (std::list<Move*>::iterator it = openList.begin(); it != openList.end(); ++it)
					{
						Move *n = dynamic_cast<Move*>(*it);
						if (n->x == succ->x && n->y == succ->y)
						{
							inOpen = n;
							break;
						}
					}
					if (inOpen == NULL) // IF succ NOT IN OpenList
					{
						// ADD succ TO OpenList
						openList.push_front(succ);
					}
					else if (succ->g >= inOpen->g) // ELSE IF succ.g >= OpenList[succ].g
					{
						delete succ;
						continue;
					}
					// cameFrom[succ] = current
					cameFrom.insert(std::make_pair(succ, current));
				}
			}
		}
	}

	// Reconstruct path:
	std::list<Coordinate> path;
	path.push_back(Coordinate(goalX, goalY));
	while (cameFrom.count(current) == 1)
	{
		current = cameFrom.find(current)->second;
		path.push_front(Coordinate(current->x, current->y));
		//printf("(%d, %d)\n", current->x, current->y);
	}

	// Delete items from closed list (free memory)
	for (std::list<Move*>::iterator it = closedList.begin(); it != closedList.end(); ++it)
	{
		Move *n = dynamic_cast<Move*>(*it);
		delete n;
	}

	/*printf("Path:\n");
	for (Coordinate c : path)
	{
		printf("%d,%d\n", c.x, c.y);
	} */

	return path;
}

int Map::Get(const int x, const int y) const
{
	if (!IsValidCoordinate(x, y))
	{
		throw std::out_of_range("x,y coordinate not within map range");
	}
	return mapGrid[y][x];
}

void Map::Set(const int x, const int y, const int val)
{
	if (!IsValidCoordinate(x, y))
	{
		throw std::out_of_range("x,y coordinate not within map range");
	}
	mapGrid[y][x] = val;
}

bool Map::IsValidCoordinate(const int x, const int y) const
{
	if (x < 0 || x >= width || y < 0 || y >= height)
	{
		return false;
	}
	return true;
}

bool Map::IsObstacle(const int x, const int y) const
{
	return Get(x, y) > 2 && Get(x, y) < 10;
}

bool Map::IsMud(const int x, const int y) const
{
	return Get(x, y) > 9;
}

bool Map::IsPointCollision(const int px, const int py)
{
	bool end = false;
	for (int x = 0; x < this->GetWidth() && !end; x++)
	for (int y = 0; y < this->GetHeight() && !end; y++)
	{
		if (IsObstacle(x, y))
		{
			double xps[] = { x * 50, x * 50, x * 50 + 50, x * 50 + 50 };
			double yps[] = { y * 50 + 50, y * 50 + 100, y * 50 + 100, y * 50 + 50 };
			if (isPointInsidePolygon(4, xps, yps, px, py))
			{
				// point is inside wall
				return true;
			}
		}
	}
	return false;
}

bool Map::IsPolygonCollision(const int num, const double xps[], const double yps[])
{
	for (int i = 0; i < num; i++)
	{
		if (IsPointCollision(xps[i], yps[i]))
			return true;
	}
	return false;
}

int Map::GetWidth()
{
	return width;
}

int Map::GetHeight()
{
	return height;
}

void Map::SpreadMud()
{
	// Discover all the muds tiles
	std::vector<Coordinate> muds;
	bool end = false;
	for (int x = 0; x < this->GetWidth(); x++)
	for (int y = 0; y < this->GetHeight(); y++)
	{
		if (IsMud(x, y))
		{
			muds.push_back(Coordinate(x, y));
		}
	}
	// pick a random mud tile
	if (muds.size() == 0)
		return;
	int i = rand() % muds.size();
	Coordinate mud = muds[i];
	// Decide whether to spread or shrink the mud - slightly favour growth
	if (rand() % 3 > 0)
	{
		// Spread..
		std::vector<Coordinate> spreadDirections;
		if (IsValidCoordinate(mud.x - 1, mud.y) && !IsObstacle(mud.x - 1, mud.y))
			spreadDirections.push_back(Coordinate(mud.x - 1, mud.y));
		if (IsValidCoordinate(mud.x, mud.y - 1) && !IsObstacle(mud.x, mud.y - 1))
			spreadDirections.push_back(Coordinate(mud.x, mud.y - 1));
		if (IsValidCoordinate(mud.x + 1, mud.y) && !IsObstacle(mud.x + 1, mud.y))
			spreadDirections.push_back(Coordinate(mud.x + 1, mud.y));
		if (IsValidCoordinate(mud.x, mud.y + 1) && !IsObstacle(mud.x, mud.y + 1))
			spreadDirections.push_back(Coordinate(mud.x, mud.y + 1));
		if (spreadDirections.size() == 0)
			return;
		int n = rand() % spreadDirections.size();
		Coordinate spreadTo = spreadDirections[n];
		Set(spreadTo.x, spreadTo.y, Get(mud.x, mud.y));
	}
	else
	{
		// Shrink..
		if (Get(mud.x, mud.y) == 10) Set(mud.x, mud.y, 0);
		else if (Get(mud.x, mud.y) == 11) Set(mud.x, mud.y, 1);
		else if (Get(mud.x, mud.y) == 12) Set(mud.x, mud.y, 2);
	}
	ReloadTileManager();
}

void Map::ReloadTileManager()
{
	tilemanager->SetSize(16, 11);
	for (int x = 0; x < 16; x++)
	for (int y = 0; y < 11; y++)
		tilemanager->SetValue(x, y, mapGrid[y][x]);
}