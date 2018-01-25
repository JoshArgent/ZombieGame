#include "HeartZombieObject.h"
#include "PlayState.h"
#include "PlayerObject.h"
#include "HeartObject.h"
#include "Psyja2Engine.h"

HeartZombieObject::HeartZombieObject(PlayState* playState, int startX, int startY, int health) : ZombieObject(playState, startX, startY, health)
{
	goalObject = playState->getPlayer();
}

HeartZombieObject::~HeartZombieObject()
{
}

void HeartZombieObject::ReachedGoal()
{
	if (goalObject == playState->getPlayer())
	{
		// hit player
		PlayerObject *player = (PlayerObject*)goalObject;
		player->Damage(0.01);
	}
	else
	{
		// must be a heart - remove it
		playState->numHearts--;
		playState->engine->RemoveObjectFromArray(goalObject);
		SetGoal(playState->getPlayer());
	}	
}

void HeartZombieObject::DoZombieUpdate()
{
	std::vector<HeartObject*> hearts = playState->engine->GetAllObjectsOfType<HeartObject>();
	if (goalObject == playState->getPlayer())
	{
		if (hearts.size() > 0)
		{
			// A heart has spawned - change the goal to the heart
			SetGoal(hearts[0]);
		}
	}
	else
	{
		// Check the current target heart still exists
		bool stillExists = false;
		for (HeartObject *heart : hearts)
		{
			if (heart == goalObject)
				stillExists = true;
		}
		if (!stillExists)
			SetGoal(playState->getPlayer());
	}
}

std::vector<ZombieColor> HeartZombieObject::GetColorOptions()
{
	std::vector<ZombieColor> colors = std::vector<ZombieColor>();
	colors.push_back(ZombieColor(0xB50D0D, 0x807769));
	return colors;
}