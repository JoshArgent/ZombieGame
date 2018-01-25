#include "BulletObject.h"
#include "Map.h"
#include "ZombieObject.h"
#include <math.h>
#include "HelperFunctions.h"
#include "Polygon.h"
#include "PlayState.h"
#include "Psyja2Engine.h"

BulletObject::BulletObject(PlayState* playState, double startX, double startY, double direction) : DisplayableObject(playState->engine), playState(playState)
{
	m_iDrawWidth = 5;
	m_iDrawHeight = 5;
	x = m_iPreviousScreenX = m_iCurrentScreenX = startX;
	y = m_iPreviousScreenY = m_iCurrentScreenY = startY;
	direction -= 2.5 * M_PI;
	double hyp = 14;
	xInc = cos(direction) * hyp;
	yInc = sin(direction) * hyp;


}

BulletObject::~BulletObject()
{
}

void BulletObject::Draw()
{
	// Do not draw if it should not be visible
	if (!IsVisible())
		return;
	unsigned int color = 0x000000;

	playState->engine->DrawScreenOval(m_iCurrentScreenX, m_iCurrentScreenY, m_iCurrentScreenX + 4, m_iCurrentScreenY + 4, color);

	StoreLastScreenPositionForUndraw();
}

void BulletObject::DoUpdate(int iCurrentTime)
{
	x += xInc;
	y += yInc;
	m_iCurrentScreenX = round(x);
	m_iCurrentScreenY = round(y);
	bool destroyBullet = false;

	// Check if the bullet hits a wall
	if (playState->mapObj->IsPointCollision(m_iCurrentScreenX, m_iCurrentScreenY))
	{
		// Collision - destroy bullet
		destroyBullet = true;
	}

	// Check if the bullet hits a zombie
	std::vector<ZombieObject*> zombies = playState->getZombies();
	for (ZombieObject* zombie : zombies)
	{
		if (destroyBullet)
			break;
		std::vector<Polygon> bounds = zombie->GetBounds();
		for (Polygon p : bounds)
		{
			if (isPointInsidePolygon(p.xps.size(), &p.xps[0], &p.yps[0], m_iCurrentScreenX, m_iCurrentScreenY))
			{
				if (zombie->GetHealth() <= 1)
				{
					playState->KillZombie(zombie);
				}
				else
				{
					zombie->SetHealth(zombie->GetHealth() - 1);
				}
				destroyBullet = true;
				break;
			} 
		}
	}


	// Boundry detection
	if (m_iCurrentScreenX < 0)
		destroyBullet = true;
	if (m_iCurrentScreenY < 0)
		destroyBullet = true;
	if (m_iCurrentScreenX + m_iDrawWidth > BASE_SCREEN_WIDTH)
		destroyBullet = true;
	if (m_iCurrentScreenY + m_iDrawHeight > BASE_SCREEN_HEIGHT)
		destroyBullet = true;

	// Destory the bullet if it has had a collision
	if (destroyBullet)
	{
		RedrawBackground();
		playState->engine->RemoveObjectFromArray(this);
	}
	else
		RedrawObjects();
}