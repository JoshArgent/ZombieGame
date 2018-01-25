#include "CoinZombieObject.h"
#include "PlayState.h"
#include "PlayerObject.h"
#include "CoinObject.h"
#include "Psyja2Engine.h"

CoinZombieObject::CoinZombieObject(PlayState* playState, int startX, int startY, int health) : ZombieObject(playState, startX, startY, health)
{
	goalObject = playState->getPlayer();
}

CoinZombieObject::~CoinZombieObject()
{
}

void CoinZombieObject::ReachedGoal()
{
	PlayerObject *player = (PlayerObject*) goalObject;
	player->Damage(0.01);
}

void CoinZombieObject::MovedTile(int tileX, int tileY)
{
	std::vector<CoinObject*> coins = playState->engine->GetAllObjectsOfType<CoinObject>();
	for (CoinObject *c : coins)
	{
		if (c->getTilePosition().x == tileX && c->getTilePosition().y == tileY)
		{
			// Zombie 'steals' the coin
			playState->engine->RemoveObjectFromArray(c);
			playState->numCoins--;
		}
	}
}

std::vector<ZombieColor> CoinZombieObject::GetColorOptions()
{
	std::vector<ZombieColor> colors = std::vector<ZombieColor>();
	colors.push_back(ZombieColor(0xE9ED66, 0xAD8832));
	return colors;
}