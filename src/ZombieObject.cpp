#pragma once
#include "ZombieObject.h"
#include "Psyja2Engine.h"
#include "PlayState.h"
#include "HelperFunctions.h"
#include "PlayerObject.h"
#include "Map.h"

//struct ZombieColor;

ZombieObject::ZombieObject(PlayState* playState, int startX, int startY, int health) : TileObject(playState, startX, startY)
{
	m_iDrawWidth = 38;
	m_iDrawHeight = 38;
	this->health = health;
}

ZombieObject::~ZombieObject()
{
}

void ZombieObject::Draw()
{
	if (skinColor == 0 || clothesColor == 0)
	{
		// Generate random zombie colors
		std::vector<ZombieColor> colorOptions = GetColorOptions();
		int n = rand() % colorOptions.size();
		skinColor = colorOptions[n].skin;
		clothesColor = colorOptions[n].clothes;
	}

	// Do not draw if it should not be visible
	if (!IsVisible())
		return;

	// Generate some commonly used points to prevent the same calculations been performed repeatedly
	double x = m_iCurrentScreenX + 2.6; // An offset to account for rotation of the zombie
	double y = m_iCurrentScreenY + 9.4;
	double width20 = 32 / (double)20; // The actual width of the zombie is 32px
	double height20 = 32 / (double)20;
	double xs[21];
	double ys[21];
	for (int i = 0; i < 21; i++)
		xs[i] = width20 * i + x;
	for (int i = 0; i < 21; i++)
		ys[i] = height20 * i + y;
	ys[20] -= 1;
	xs[20] -= 1;

	/* Draw the zombie */
	// Shoulders
	double xp1[] = { xs[0], xs[20], xs[20], xs[0] };
	double yp1[] = { ys[6], ys[6], ys[10], ys[10] };
	rotatePolygon(4, xp1, yp1, xs[10], ys[5], angle);
	playState->engine->DrawScreenPolygon(4, xp1, yp1, clothesColor);
	// Left arm
	double xp2[] = { xs[1] - 1, xs[5] - 1, xs[5] - 1, xs[1] - 1 };
	double yp2[] = { ys[1], ys[1], ys[6], ys[6] };
	rotatePolygon(4, xp2, yp2, xs[10], ys[5], angle);
	playState->engine->DrawScreenPolygon(4, xp2, yp2, clothesColor);
	// Right arm
	double xp3[] = { xs[15], xs[19], xs[19], xs[15] };
	double yp3[] = { ys[1], ys[1], ys[6], ys[6] };
	rotatePolygon(4, xp3, yp3, xs[10], ys[5], angle);
	playState->engine->DrawScreenPolygon(4, xp3, yp3, clothesColor);
	// Left hand
	double xp4[] = { xs[1] - 1, xs[5] - 1, xs[5] - 1, xs[1] - 1 };
	double yp4[] = { ys[0], ys[0], ys[1], ys[1] };
	rotatePolygon(4, xp4, yp4, xs[10], ys[5], angle);
	playState->engine->DrawScreenPolygon(4, xp4, yp4, skinColor);
	// Right hand
	double xp5[] = { xs[15], xs[19], xs[19], xs[15] };
	double yp5[] = { ys[0], ys[0], ys[1], ys[1] };
	rotatePolygon(4, xp5, yp5, xs[10], ys[5], angle);
	playState->engine->DrawScreenPolygon(4, xp5, yp5, skinColor);
	// Head
	double xp6[] = { xs[6], xs[14], xs[14], xs[6] };
	double yp6[] = { ys[4], ys[4], ys[12], ys[12] };
	rotatePolygon(4, xp6, yp6, xs[10], ys[5], angle);
	playState->engine->DrawScreenPolygon(4, xp6, yp6, skinColor);
	
	StoreLastScreenPositionForUndraw();
}

void ZombieObject::DoUpdate(int iCurrentTime)
{
	DoZombieUpdate();

	// Get the centre point of zombie
	int zombieX = GetXCentre();
	int zombieY = GetYCentre();

	// Is the zombie in mud?
	bool inMud = playState->mapObj->IsMud(getTilePosition().x, getTilePosition().y);
	double maxV = SPEED;
	if (inMud)
	{
		maxV = 1.0; // zombies will be slightly better in mud than humans ;)
	}

	// See if the player location has changed - recalculate path
	Coordinate zombieCoord = this->getTilePosition();
	Coordinate goalCoord = goalObject->getTilePosition();
	if (goalCoord.x != goalTileCache.x || goalCoord.y != goalTileCache.y)
	{
		
		pathCache.clear();
		pathCache = playState->mapObj->calculatePath(zombieCoord.x, zombieCoord.y, goalCoord.x, goalCoord.y);
		goalTileCache = goalCoord;
		pathCache.pop_front(); // remove the first node (it's the start node)
	}

	double goalX = pathCache.front().x * 50 + 25;
	double goalY = pathCache.front().y * 50 + 50 + 25;

	// See if the front node in the path has been reached - remove it and carry on
	if (goalX < zombieX + 5 && goalX > zombieX - 5 && goalY < zombieY + 5 && goalY > zombieY - 5)
	{
		pathCache.pop_front();
		goalX = pathCache.front().x * 50 + 25;
		goalY = pathCache.front().y * 50 + 50 + 25;	
	}

	// If the path is empty then the zombie is 1 tile next to the player - go direct to them
	if (pathCache.size() == 0)
	{
		goalX = goalObject->GetXCentre();
		goalY = goalObject->GetYCentre();
	}
	
	double difX = goalX - zombieX;
	double difY = goalY - zombieY;

	if (pathCache.size() == 0 && difX < 25 && difX > -25 && difY < 30 && difY > -25)
	{
		// Reached goal!
		ReachedGoal();
	} 

	double hyp = sqrt(difX*difX + difY*difY);
	if (hyp != 0)
	{
		// Calculate change in x and y - account for rounding (iX and iY should never round to 0)
		difX /= hyp;
		difY /= hyp;
		double iX = difX * maxV;
		double iY = difY * maxV;
		if (iX < 0.5 && iX > 0)
			iX = 1;
		if (iX > -0.5 && iX < 0)
			iX = -1;
		if (iY < 0.5 && iY > 0)
			iY = 1;
		if (iY > -0.5 && iY < 0)
			iY = -1;
		zombieX += round(iX);
		zombieY += round(iY);
		angle = getAngle(zombieX, zombieY, goalX, goalY);
	}

	Coordinate before = this->getTilePosition();

	// Recalculate actual position from centre point..
	m_iCurrentScreenX = zombieX - (m_iDrawWidth / 2);
	m_iCurrentScreenY = zombieY - (m_iDrawHeight / 2);

	Coordinate after = this->getTilePosition();
	if (before.x != after.x || before.y != after.y)
	{
		MovedTile(after.x, after.y);
	}

	// Game boundary detection
	if (m_iCurrentScreenX < 0)
		m_iCurrentScreenX = 0;
	if (m_iCurrentScreenY < 0)
		m_iCurrentScreenY = 50;
	if (m_iCurrentScreenX + m_iDrawWidth > BASE_SCREEN_WIDTH)
		m_iCurrentScreenX = BASE_SCREEN_WIDTH - m_iDrawWidth;
	if (m_iCurrentScreenY + m_iDrawHeight > BASE_SCREEN_HEIGHT)
		m_iCurrentScreenY = BASE_SCREEN_HEIGHT - m_iDrawHeight;

	// Redraw the player
	RedrawObjects();
}

// Sets the target object for the zombie to track
void ZombieObject::SetGoal(TileObject *goalObject)
{
	if (goalObject != this->goalObject)
	{
		pathCache.clear();
		this->goalObject = goalObject;
	}
}

// Called when the zombie enters a new tile
void ZombieObject::MovedTile(int tileX, int tileY)
{

}

// Called when the zombie reaches it's goal object
void ZombieObject::ReachedGoal()
{
	
}

// Returns a vector list of possible zombie color schemes
std::vector<ZombieColor> ZombieObject::GetColorOptions()
{
	std::vector<ZombieColor> colors = std::vector<ZombieColor>();
	colors.push_back(ZombieColor(0x4D897C, 0x5B7674));
	colors.push_back(ZombieColor(0x2AA82C, 0x2A50A8));
	return colors;
}

void ZombieObject::DoZombieUpdate()
{

}

// Returns the health of the zombie
int ZombieObject::GetHealth()
{
	return health;
}

// Sets the health of the zombie
void ZombieObject::SetHealth(int health)
{
	this->health = health;
}

std::vector<Polygon> ZombieObject::GetBounds()
{
	std::vector<Polygon> bounds;

	// Generate some commonly used points to prevent the same calculations been performed repeatedly
	double x = m_iCurrentScreenX + 2.6; // An offset to account for rotation of the zombie
	double y = m_iCurrentScreenY + 9.4;
	double width20 = 32 / (double)20; // The actual width of the zombie is 32px
	double height20 = 32 / (double)20;
	double xs[21];
	double ys[21];
	for (int i = 0; i < 21; i++)
		xs[i] = width20 * i + x;
	for (int i = 0; i < 21; i++)
		ys[i] = height20 * i + y;
	ys[20] -= 1;
	xs[20] -= 1;

	// Shoulders
	std::vector<double> xp1 = { xs[0], xs[20], xs[20], xs[0] };
	std::vector<double> yp1 = { ys[6], ys[6], ys[10], ys[10] };
	rotatePolygon(xp1, yp1, xs[10], ys[5], angle);
	bounds.push_back(Polygon(xp1, yp1));
	// Left arm
	std::vector<double> xp2 = { xs[1] - 1, xs[5] - 1, xs[5] - 1, xs[1] - 1 };
	std::vector<double> yp2 = { ys[1], ys[1], ys[6], ys[6] };
	rotatePolygon(xp2, yp2, xs[10], ys[5], angle);
	bounds.push_back(Polygon(xp2, yp2));
	// Right arm
	std::vector<double> xp3 = { xs[15], xs[19], xs[19], xs[15] };
	std::vector<double> yp3 = { ys[1], ys[1], ys[6], ys[6] };
	rotatePolygon(xp3, yp3, xs[10], ys[5], angle);
	bounds.push_back(Polygon(xp3, yp3));
	// Left hand
	std::vector<double> xp4 = { xs[1] - 1, xs[5] - 1, xs[5] - 1, xs[1] - 1 };
	std::vector<double> yp4 = { ys[0], ys[0], ys[1], ys[1] };
	rotatePolygon(xp4, yp4, xs[10], ys[5], angle);
	bounds.push_back(Polygon(xp4, yp4));
	// Right hand
	std::vector<double> xp5 = { xs[15], xs[19], xs[19], xs[15] };
	std::vector<double> yp5 = { ys[0], ys[0], ys[1], ys[1] };
	rotatePolygon(xp5, yp5, xs[10], ys[5], angle);
	bounds.push_back(Polygon(xp5, yp5));
	// Head
	std::vector<double> xp6 = { xs[6], xs[14], xs[14], xs[6] };
	std::vector<double> yp6 = { ys[4], ys[4], ys[12], ys[12] };
	rotatePolygon(xp6, yp6, xs[10], ys[5], angle);
	bounds.push_back(Polygon(xp6, yp6));

	return bounds;
}