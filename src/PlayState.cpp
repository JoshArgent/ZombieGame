#include "PlayState.h"
#include <sstream>
#include "Map.h"
#include "PlayerObject.h"
#include "CoinObject.h"
#include "ZombieObject.h"
#include "Psyja2TileManager.h"
#include "BulletObject.h"
#include "HealthBarObject.h"
#include "HeartObject.h"
#include "JPGImage.h"
#include "Psyja2Engine.h"
#include "PauseState.h"
#include "Round.h"
#include "RoundCompleteState.h"
#include "GameoverState.h"
#include "DiedState.h"
#include "NormalZombieObject.h"
#include "CoinZombieObject.h"
#include "HeartZombieObject.h"
#include "SpriteManager.h"


PlayState::PlayState(Psyja2Engine *engine, int score, int lives, int round, const char *levelFile) : State(engine)
{
	tilemanager = new Psyja2TileManager();
	mapObj = new Map(tilemanager, levelFile);
	player = new PlayerObject(this);
	player->setTilePosition(mapObj->spawnX, mapObj->spawnY);
	player->lives = lives;
	player->score = score;
	this->round = round;
	Round roundObj = Round(round);
	this->zombieRate = roundObj.GetZombieSpawnRate();
	this->zombieLimit = roundObj.GetZombieSpawnLimit();
	this->zombiesLeft = roundObj.GetNumZombies();
	this->zombieHealth = roundObj.GetZombieHealth();
}

PlayState::~PlayState()
{
	delete mapObj;
	delete tilemanager;
}

void PlayState::SetupBackgroundBuffer()
{
	// Display the map	
	tilemanager->SetBaseTilesPositionOnScreen(0, 50);
	tilemanager->DrawAllTiles(engine, engine->GetBackground(), 0, 0, 16, 11);
	
	// Draw any blood stains
	int i = 0;
	for (Coordinate *c : bloodStains)
	{
		double phase = bloodStainPhases[i];
		ImageData bloodImg;
		bloodImg.ResizeFrom(engine->spriteManager->GetBlood(), engine->spriteManager->GetBlood()->GetWidth() * phase, engine->spriteManager->GetBlood()->GetHeight() * phase, true);
		int offset = 25 - (engine->spriteManager->GetBlood()->GetWidth() * phase) / 2;
		bloodImg.RenderImageWithMask(engine->GetBackground(), 0, 0, c->x * 50 + offset, c->y * 50 + 50 + offset, bloodImg.GetWidth(), bloodImg.GetHeight());
		i++;
	}

	// Draw score bar
	engine->DrawBackgroundRectangle(0, 0, engine->GetScreenWidth(), 50, 0xFFFFFF);

	// Draw heart and coin icons for score
	engine->spriteManager->GetHeart()[0]->RenderImageWithMask(engine->GetBackground(), 0, 0, engine->GetScreenWidth() - 70, 28, engine->spriteManager->GetHeart()[0]->GetWidth(), engine->spriteManager->GetHeart()[0]->GetHeight());
	engine->spriteManager->GetCoins()[0]->RenderImageWithMask(engine->GetBackground(), 0, 0, engine->GetScreenWidth() - 70, 5, engine->spriteManager->GetCoins()[0]->GetWidth(), engine->spriteManager->GetCoins()[0]->GetHeight());
}

int PlayState::InitialiseObjects()
{
	engine->DrawableObjectsChanged();
	engine->DestroyOldObjects();
	engine->CreateObjectArray(10);

	// Spawn player
	engine->StoreObjectInArray(player);

	// Health bar
	engine->StoreObjectInArray(new HealthBarObject(this));

	return 2;
}

void PlayState::MouseMoved(int iX, int iY)
{
	double angle = getAngle(player->GetXCentre(), player->GetYCentre(), iX, iY);
	player->SetAngle(angle);
}

void PlayState::MouseDown(int iButton, int iX, int iY)
{
	double angle = getAngle(player->GetXCentre(), player->GetYCentre(), iX, iY);
	BulletObject *bullet = new BulletObject(this, player->GetXCentre(), player->GetYCentre(), angle);
	engine->StoreObjectInArray(bullet);
	
}

void PlayState::DrawStrings()
{
	engine->CopyBackgroundPixels(0, 0, engine->GetScreenWidth(), engine->GetScreenHeight());
	std::ostringstream ss;
	ss << "x" << std::to_string(getPlayer()->score);
	engine->DrawScreenString(engine->GetScreenWidth() - 50, 4, ss.str().c_str(), 0x000000, NULL);
	ss.str("");
	ss << "x" << std::to_string(getPlayer()->lives);
	engine->DrawScreenString(engine->GetScreenWidth() - 50, 30, ss.str().c_str(), 0x000000, NULL);
	ss.str("");
	ss << "Round: " << std::to_string(round);
	Font *font = engine->GetFont("Millimetre-Bold.otf", 20);
	engine->DrawScreenString(10, 15, ss.str().c_str(), 0xFF0000, font);
}

void PlayState::GameAction()
{
	// If too early to act then do nothing
	if (!engine->IsTimeToActWithSleep())
		return;

	// Spawn zombies
	int t = 100 * (1 - zombieRate);
	if (numZombies < std::min(zombieLimit, zombiesLeft) && rand() % t == 1)
	{
		// Select the type of zombie:
		int type = rand() % 10;
		// Choose where to spawn it
		if (rand() % 2 == 1)
		{
			if (type < 7)
				engine->StoreObjectInArray(new NormalZombieObject(this, 0, 5, zombieHealth));
			else if (type < 9)
				engine->StoreObjectInArray(new CoinZombieObject(this, 0, 5, zombieHealth));
			else
				engine->StoreObjectInArray(new HeartZombieObject(this, 0, 5, zombieHealth));
		}
		else
		{
			if (type < 7)
				engine->StoreObjectInArray(new NormalZombieObject(this, 15, 5, zombieHealth));
			else if (type < 9)
				engine->StoreObjectInArray(new CoinZombieObject(this, 15, 5, zombieHealth));
			else
				engine->StoreObjectInArray(new HeartZombieObject(this, 15, 5, zombieHealth));
		}
		numZombies++;
	}

	// Spawn coins
	t = 100 * (1 - coinRate);
	if (numCoins < coinLimit && rand() % t == 1)
	{
		while (true)
		{
			int x = rand() % 16;
			int y = rand() % 11;
			if (!mapObj->IsObstacle(x, y))
			{
				CoinObject *coin = new CoinObject(this, x, y);
				engine->StoreObjectInArray(coin);
				numCoins++;
				break;
			}
		}
	}

	// Spawn hearts
	t = 10000 * (1 - heartRate);
	if (numHearts < heartLimit && rand() % t == 1)
	{
		while (true)
		{
			int x = rand() % 16;
			int y = rand() % 11;
			if (!mapObj->IsObstacle(x, y))
			{
				HeartObject *heart = new HeartObject(this, x, y);
				engine->StoreObjectInArray(heart);
				numHearts++;
				break;
			}
		}
	}

	// Spread any mud
	if (rand() % 1000 == 1)
	{
		mapObj->SpreadMud();
		// redraw the background;
		SetupBackgroundBuffer();
		engine->Redraw(true);
	}

	// Shrink any blood stains
	int i = 0;
	bool redraw = bloodStainPhases.size() > 0;
	for (double phase : bloodStainPhases)
	{
		phase -= 0.001;
		bloodStainPhases[i] = phase;
		if (phase < 0.05)
		{
			// The lowest phase will always be at the back
			bloodStainPhases.pop_back();
			bloodStains.pop_back();
		}
		i++;
	}
	if (redraw)
	{
		SetupBackgroundBuffer();
		engine->Redraw(true);
	}

	// Don't act for another 15 ticks
	engine->SetTimeToAct(15);

	// Tell all objects to update themselves.
	// If they need the screen to redraw then they should say so, so that GameRender() will
	// call the relevant function later.
	engine->UpdateAllObjects(engine->GetTime());

	// See if the round is complete
	if (zombiesLeft <= 0)
	{
		PlayState *nextState = new PlayState(engine, player->score, player->lives, this->round + 1, engine->levels[engine->NextLevel()].c_str());
		RoundCompleteState *roundComplete = new RoundCompleteState(engine, this, nextState);

		// Round is complete. Load a new play state with the round increased..
		engine->SwitchState(roundComplete, true, true);
		return;
	}

	// See if the player has died
	if (player->GetHealth() <= 0)
	{
		player->SetVisible(false);
		if (player->lives > 0)
		{
			// deduct a life and respawn
			engine->SwitchState(new DiedState(engine, this), true, true);
			player->SetHealth(1);
			player->setTilePosition(mapObj->spawnX, mapObj->spawnY);
			player->lives--;
		}
		else
		{
			// game over
			engine->SwitchState(new GameoverState(engine, this), true, true);
		}
	}
}

void PlayState::KeyDown(int iKeyCode)
{
	if (iKeyCode == SDLK_SPACE)
	{
		// Enter pause state
		engine->SwitchState(new PauseState(engine, this), true, true);
	}
}

PlayerObject* PlayState::getPlayer()
{
	return player;
}

std::vector<CollectableObject*> PlayState::getCollectables()
{
	return engine->GetAllObjectsOfType<CollectableObject>();
}

std::vector<ZombieObject*> PlayState::getZombies()
{
	return engine->GetAllObjectsOfType<ZombieObject>();
}

void PlayState::KillZombie(ZombieObject* zombie)
{
	// create blood stain
	bloodStains.push_front(new Coordinate(zombie->getTilePosition().x, zombie->getTilePosition().y));
	bloodStainPhases.push_front(1);
	zombie->RedrawBackground();
	engine->RemoveObjectFromArray(zombie);
	numZombies--;
	zombiesLeft--;
}