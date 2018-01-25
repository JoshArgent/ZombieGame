#include "NormalZombieObject.h"
#include "PlayerObject.h"
#include "PlayState.h"

NormalZombieObject::NormalZombieObject(PlayState* playState, int startX, int startY, int health) : ZombieObject(playState, startX, startY, health)
{
	goalObject = playState->getPlayer();
}

NormalZombieObject::~NormalZombieObject()
{
}

void NormalZombieObject::ReachedGoal()
{
	PlayerObject *player = (PlayerObject*) goalObject;
	player->Damage(0.01);
}