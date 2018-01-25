#include "CoinObject.h"
#include "Psyja2Engine.h"
#include "PlayerObject.h"
#include "PlayState.h"
#include "SpriteManager.h"


CoinObject::CoinObject(PlayState* playState, int startX, int startY) : CollectableObject(playState, startX, startY)
{
}

CoinObject::~CoinObject()
{
}

void CoinObject::Collect()
{
	playState->getPlayer()->score++;
	playState->numCoins--;
}

inline std::vector<ImageData*>& CoinObject::GetSprites()
{
	return playState->engine->spriteManager->GetCoins();
}
