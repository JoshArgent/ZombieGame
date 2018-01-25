#include "HeartObject.h"
#include "Psyja2Engine.h"
#include "PlayerObject.h"
#include "PlayState.h"
#include "SpriteManager.h"


HeartObject::HeartObject(PlayState *playState, int startX, int startY) : CollectableObject(playState, startX, startY)
{
}

HeartObject::~HeartObject()
{
}

void HeartObject::Collect()
{
	playState->getPlayer()->lives++;
	playState->numHearts--;
}

inline std::vector<ImageData*>& HeartObject::GetSprites()
{
	return playState->engine->spriteManager->GetHeart();
}
