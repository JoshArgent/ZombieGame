#pragma once
#include "PlayerObject.h"
#include "HelperFunctions.h"
#include "Psyja2Engine.h"
#include "CoinObject.h"
#include "PlayState.h"
#include "Map.h"
#include <vector>
#include <algorithm>
using std::min;
using std::max;

PlayerObject::PlayerObject(PlayState* playState) : TileObject(playState, 1, 1)
{
	m_iDrawWidth = 42;
	m_iDrawHeight = 42;
}

PlayerObject::~PlayerObject()
{
}

void PlayerObject::Draw()
{
	// Do not draw if it should not be visible
	if (!IsVisible())
		return;
	unsigned int color = 0x0000FF;

	/*double xps[5] = { m_iCurrentScreenX + 15, m_iCurrentScreenX + 25, m_iCurrentScreenX + 25, m_iCurrentScreenX + 5, m_iCurrentScreenX + 5 };
	double yps[5] = { m_iCurrentScreenY + 5, m_iCurrentScreenY + 10, m_iCurrentScreenY + 25, m_iCurrentScreenY + 25, m_iCurrentScreenY + 10 };
	rotatePolygon(5, xps, yps, m_iCurrentScreenX + 15, m_iCurrentScreenY + 15, angle);
	engine->DrawScreenPolygon(5, xps, yps, color);	 */

	// Generate some commonly used points to prevent the same calculations been performed repeatedly
	double x = m_iCurrentScreenX + 5; // An offset to account for rotation of the player
	double y = m_iCurrentScreenY + 12;
	double width20 = 32 / (double)20; // The actual width of the player is 32px
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
	double xp1[] = { xs[0], xs[20], xs[20], xs[0] };
	double yp1[] = { ys[9], ys[9], ys[13], ys[13] };
	rotatePolygon(4, xp1, yp1, xs[10], ys[5], angle);
	playState->engine->DrawScreenPolygon(4, xp1, yp1, clothesColor);
	// Left arm
	double xp2[] = { xs[1] - 1, xs[5] - 1, xs[5] - 1, xs[1] - 1 };
	double yp2[] = { ys[4], ys[4], ys[9], ys[9] };
	rotatePolygon(4, xp2, yp2, xs[10], ys[5], angle);
	playState->engine->DrawScreenPolygon(4, xp2, yp2, clothesColor);
	// Right arm
	double xp3[] = { xs[15], xs[19], xs[19], xs[15] };
	double yp3[] = { ys[4], ys[4], ys[9], ys[9] };
	rotatePolygon(4, xp3, yp3, xs[10], ys[5], angle);
	playState->engine->DrawScreenPolygon(4, xp3, yp3, clothesColor);
	// Left hand
	double xp4[] = { xs[1] - 1, xs[5] - 1, xs[5] - 1, xs[1] - 1 };
	double yp4[] = { ys[3], ys[3], ys[4], ys[4] };
	rotatePolygon(4, xp4, yp4, xs[10], ys[5], angle);
	playState->engine->DrawScreenPolygon(4, xp4, yp4, skinColor);
	// Right hand
	double xp5[] = { xs[15], xs[19], xs[19], xs[15] };
	double yp5[] = { ys[3], ys[3], ys[4], ys[4] };
	rotatePolygon(4, xp5, yp5, xs[10], ys[5], angle);
	playState->engine->DrawScreenPolygon(4, xp5, yp5, skinColor);
	// Head
	double xp6[] = { xs[6], xs[14], xs[14], xs[6] };
	double yp6[] = { ys[7], ys[7], ys[15], ys[15] };
	rotatePolygon(4, xp6, yp6, xs[10], ys[5], angle);
	playState->engine->DrawScreenPolygon(4, xp6, yp6, skinColor);
	// Hair
	double xp7[] = { xs[6], xs[14], xs[14], xs[6] };
	double yp7[] = { ys[8], ys[8], ys[15], ys[15] };
	rotatePolygon(4, xp7, yp7, xs[10], ys[5], angle);
	playState->engine->DrawScreenPolygon(4, xp7, yp7, hairColor);
	// Gun
	double xp8[] = { xs[14], xs[18], xs[18], xs[14] };
	double yp8[] = { ys[0], ys[0], ys[11], ys[11] };
	rotatePolygon(4, xp8, yp8, xs[10], ys[5], angle);
	playState->engine->DrawScreenPolygon(4, xp8, yp8, 0x000000);

	StoreLastScreenPositionForUndraw();
}

void PlayerObject::SetAngle(double angle)
{
	double beforeAngle = this->angle;
	
	// Check for hitting edges of walls
	//DetectCollisions(m_iCurrentScreenX, m_iCurrentScreenY, beforeAngle, m_iCurrentScreenX, m_iCurrentScreenY, angle);

	this->angle = angle;
}

void PlayerObject::DoUpdate(int iCurrentTime)
{
	// Save the before x/y position
	double beforeY = m_iCurrentScreenY;
	double beforeX = m_iCurrentScreenX;

	// Is the player in mud?
	bool inMud = playState->mapObj->IsMud(getTilePosition().x, getTilePosition().y);
	double a = ACCELERATION;
	double maxV = MAX_SPEED;
	if (inMud)
	{
		a /= 3;
		maxV /= 3;
	}

	// Handle Y axis movement
	if (playState->engine->IsKeyPressed(SDLK_s))
	{
		velocityY += a;
		if (velocityY > maxV)
			velocityY = maxV;
	}
	else if (playState->engine->IsKeyPressed(SDLK_w))
	{
		velocityY -= a;
		if (velocityY < -maxV)
			velocityY = -maxV;
	}
	else
	{
		if (velocityY < 0)
		{
			velocityY += a;
			if (velocityY >= 0)
				velocityY = 0;
		}
		else
		{
			velocityY -= a;
			if (velocityY <= 0)
				velocityY = 0;
		}
	}
	// Handle X axis movement
	if (playState->engine->IsKeyPressed(SDLK_d))
	{
		velocityX += a;
		if (velocityX > maxV)
			velocityX = maxV;
	}
	else if (playState->engine->IsKeyPressed(SDLK_a))
	{
		velocityX -= a;
		if (velocityX < -maxV)
			velocityX = -maxV;
	}
	else
	{
		if (velocityX < 0)
		{
			velocityX += a;
			if (velocityX >= 0)
				velocityX = 0;
		}
		else
		{
			velocityX -= a;
			if (velocityX <= 0)
				velocityX = 0;
		}
	}
	// Move the player
	m_iCurrentScreenY += velocityY;
	m_iCurrentScreenX += velocityX;

	// Game boundary detection
	if (m_iCurrentScreenX < 0)
		m_iCurrentScreenX = 0;
	if (m_iCurrentScreenY < 0)
		m_iCurrentScreenY = 0;
	if (m_iCurrentScreenX + m_iDrawWidth > BASE_SCREEN_WIDTH)
		m_iCurrentScreenX = BASE_SCREEN_WIDTH - m_iDrawWidth;
	if (m_iCurrentScreenY + m_iDrawHeight > BASE_SCREEN_HEIGHT)
		m_iCurrentScreenY = BASE_SCREEN_HEIGHT - m_iDrawHeight;

	// Check for hitting edges of walls
	DetectCollisions(beforeX, beforeY, angle, m_iCurrentScreenX, m_iCurrentScreenY, angle);

	// See if a collectable has been collected
	std::vector<CollectableObject*> objs = playState->engine->GetAllObjectsOfType<CollectableObject>();
	for (int n = 0; n < objs.size(); n++)
	{
		CollectableObject *obj = objs[n];
		if (m_iCurrentScreenX < obj->GetXCentre() && m_iCurrentScreenX + m_iDrawWidth > obj->GetXCentre()
			&& m_iCurrentScreenY < obj->GetYCentre() && m_iCurrentScreenY + m_iDrawHeight > obj->GetYCentre())
		{
			obj->CollectObject();
		}
	}

	// Regenerate health
	if (health < 1)
		health += 0.002;

	// Redraw the player
	RedrawObjects();
}

void PlayerObject::DetectCollisions(int orgX, int orgY, double orgAngle, int& newX, int& newY, double& newAngle)
{
	// Generate some commonly used points to prevent the same calculations been performed repeatedly
	double x = newX + 5; // An offset to account for rotation of the player
	double y = newY + 12;
	double width20 = 32 / (double)20; // The actual width of the player is 32px
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
	double xp1[] = { xs[0], xs[20], xs[20], xs[0] };
	double yp1[] = { ys[9], ys[9], ys[13], ys[13] };
	rotatePolygon(4, xp1, yp1, xs[10], ys[5], angle);
	// Left arm
	double xp2[] = { xs[1] - 1, xs[5] - 1, xs[5] - 1, xs[1] - 1 };
	double yp2[] = { ys[4], ys[4], ys[9], ys[9] };
	rotatePolygon(4, xp2, yp2, xs[10], ys[5], angle);
	// Right arm
	double xp3[] = { xs[15], xs[19], xs[19], xs[15] };
	double yp3[] = { ys[4], ys[4], ys[9], ys[9] };
	rotatePolygon(4, xp3, yp3, xs[10], ys[5], angle);
	// Left hand
	double xp4[] = { xs[1] - 1, xs[5] - 1, xs[5] - 1, xs[1] - 1 };
	double yp4[] = { ys[3], ys[3], ys[4], ys[4] };
	rotatePolygon(4, xp4, yp4, xs[10], ys[5], angle);
	// Right hand
	double xp5[] = { xs[15], xs[19], xs[19], xs[15] };
	double yp5[] = { ys[3], ys[3], ys[4], ys[4] };
	rotatePolygon(4, xp5, yp5, xs[10], ys[5], angle);
	// Head
	double xp6[] = { xs[6], xs[14], xs[14], xs[6] };
	double yp6[] = { ys[7], ys[7], ys[15], ys[15] };
	rotatePolygon(4, xp6, yp6, xs[10], ys[5], angle);
	// Hair
	double xp7[] = { xs[6], xs[14], xs[14], xs[6] };
	double yp7[] = { ys[8], ys[8], ys[15], ys[15] };
	rotatePolygon(4, xp7, yp7, xs[10], ys[5], angle);
	// Gun
	double xp8[] = { xs[14], xs[18], xs[18], xs[14] };
	double yp8[] = { ys[0], ys[0], ys[11], ys[11] };
	rotatePolygon(4, xp8, yp8, xs[10], ys[5], angle);
	double minX = minElement(4, xp1);
	double maxX = maxElement(4, xp1);
	double minY = minElement(4, yp1);
	double maxY = maxElement(4, yp1);
	minX = min(minElement(4, xp2), minX);
	minX = min(minElement(4, xp3), minX);
	minX = min(minElement(4, xp4), minX);
	minX = min(minElement(4, xp5), minX);
	minX = min(minElement(4, xp6), minX);
	minX = min(minElement(4, xp7), minX);
	minX = min(minElement(4, xp8), minX);
	maxX = max(maxElement(4, xp2), maxX);
	maxX = max(maxElement(4, xp3), maxX);
	maxX = max(maxElement(4, xp4), maxX);
	maxX = max(maxElement(4, xp5), maxX);
	maxX = max(maxElement(4, xp6), maxX);
	maxX = max(maxElement(4, xp7), maxX);
	maxX = max(maxElement(4, xp8), maxX);
	minY = min(minElement(4, yp2), minY);
	minY = min(minElement(4, yp3), minY);
	minY = min(minElement(4, yp4), minY);
	minY = min(minElement(4, yp5), minY);
	minY = min(minElement(4, yp6), minY);
	minY = min(minElement(4, yp7), minY);
	minY = min(minElement(4, yp8), minY);
	maxY = max(maxElement(4, yp2), maxY);
	maxY = max(maxElement(4, yp3), maxY);
	maxY = max(maxElement(4, yp4), maxY);
	maxY = max(maxElement(4, yp5), maxY);
	maxY = max(maxElement(4, yp6), maxY);
	maxY = max(maxElement(4, yp7), maxY);
	maxY = max(maxElement(4, yp8), maxY);

	int minTileX = minX / 50;
	int maxTileX = maxX / 50;
	int minTileY = minY / 50 - 1;
	int maxTileY = maxY / 50 - 1;
	const double dampenFactor = 0.4;

	if (newX < orgX) // left movement
	{
		if (playState->mapObj->IsObstacle(minTileX, maxTileY) || playState->mapObj->IsObstacle(minTileX, minTileY))
		{
			newX = orgX;
			velocityX = -velocityX * dampenFactor;
		}
	}
	else if (newX > orgX) // right movement
	{
		if (playState->mapObj->IsObstacle(maxTileX, maxTileY) || playState->mapObj->IsObstacle(maxTileX, minTileY))
		{
			newX = orgX;
			velocityX = -velocityX * dampenFactor;
		}
	}
	if (newY < orgY) // up movement
	{
		if (playState->mapObj->IsObstacle(minTileX, minTileY) || playState->mapObj->IsObstacle(maxTileX, minTileY))
		{
			newY = orgY;
			velocityY = -velocityY * dampenFactor;
		}
	}
	else if (newY > orgY) // down movement
	{
		if (playState->mapObj->IsObstacle(minTileX, maxTileY) || playState->mapObj->IsObstacle(maxTileX, maxTileY))
		{
			newY = orgY;
			velocityY = -velocityY * dampenFactor;
		}
	}
	if (newX == orgX && newY == orgY) // angle change
	{
		if (playState->mapObj->IsObstacle(minTileX, maxTileY) || playState->mapObj->IsObstacle(maxTileX, maxTileY) ||
			playState->mapObj->IsObstacle(minTileX, minTileY) || playState->mapObj->IsObstacle(maxTileX, minTileY))
		{
			newAngle = orgAngle;
		}
	}	
}

void PlayerObject::Damage(double amount)
{
	health -= amount;
	if (health < 0)
	{
		// Player dead
		health = 0;
	}
}

double PlayerObject::GetHealth()
{
	return health;
}

void PlayerObject::SetHealth(double health)
{
	this->health = health;
}