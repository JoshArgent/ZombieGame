#pragma once
#include "State.h"
#include <vector>
#include <deque>

class PlayerObject;
class ZombieObject;
class CollectableObject;
class Map;
class Psyja2TileManager;
class ImageData;
class Coordinate;

class PlayState : public State
{
public:
	PlayState(Psyja2Engine *engine, int score, int lives, int round, const char *levelFile);
	~PlayState();
	void SetupBackgroundBuffer();
	int InitialiseObjects();
	void MouseMoved(int iX, int iY);
	void MouseDown(int iButton, int iX, int iY);
	void DrawStrings();
	void GameAction();
	void KeyDown(int iKeyCode);
	Map *mapObj;
	PlayerObject *getPlayer();
	std::vector<CollectableObject*> getCollectables();
	std::vector<ZombieObject*> getZombies();
	void KillZombie(ZombieObject* zombie);
	int numZombies = 0; // number of spawned zombies
	int numCoins = 0; // number of spawned coins
	int numHearts = 0; // number of spawn hearts
	int round = 0; // current round
	double zombieRate = 0.5; // spawn rate 0 - 0.9 [1 = a lot, 0 = none]
	int zombieLimit = 20; // max number of zombies allowed to spawn
	int zombiesLeft = 10; // number of zombies left till round is over
	int zombieHealth = 1; // the starting health of zombies for this round;
	const double coinRate = 0.02; // spawn rate 0 - 0.9 [0.9 = a lot, 0 = none]
	const int coinLimit = 10; // max number of coins allowed to spawn
	const double heartRate = 0.01; // spawn rate 0 - 1 [1 = a lot, 0 = none]
	const int heartLimit = 1; // max number of hearts allowed to spawn
private:
	Psyja2TileManager *tilemanager;
	PlayerObject *player;
	std::deque<Coordinate*> bloodStains;
	std::deque<double> bloodStainPhases;
};

