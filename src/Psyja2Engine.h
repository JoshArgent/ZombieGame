#pragma once
#include "BaseEngine.h"
#include "header.h"

#define BASE_SCREEN_WIDTH 800
#define BASE_SCREEN_HEIGHT 600

class State;
class SpriteManager;

class Psyja2Engine : public BaseEngine
{
public:
	Psyja2Engine();
	~Psyja2Engine();
	void SetupBackgroundBuffer();
	int InitialiseObjects();
	void MouseMoved(int iX, int iY);
	void MouseDown(int iButton, int iX, int iY);
	void DrawStrings();
	void DrawStringsOnTop();
	void GameAction();
	void KeyDown(int iKeyCode);
	/* Switches the state - use keepState=true if you intend to switch back to the old state */
	void SwitchState(State *state, bool keepState, bool keepObjects);
	/* Vector list of existing level files */
	std::vector<std::string> levels;

	inline int NextLevel()
	{
		currentLevel++;
		if (currentLevel >= levels.size())
			currentLevel = 0;
		return currentLevel;
	}

	inline void ResetLevel()
	{
		currentLevel = -1;
	}

private:
	State *state = NULL;
	int currentLevel = -1;
public:
	SpriteManager *spriteManager;
};

