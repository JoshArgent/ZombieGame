#pragma once
#include "State.h"

class PlayState;

class GameoverState : public State
{
public:
	GameoverState(Psyja2Engine *engine, PlayState *playState);
	~GameoverState();
	void SetupBackgroundBuffer();
	void DrawStrings();
	void DrawStringsOnTop();
	void GameAction();
	void KeyDown(int iKeyCode);
private:
	PlayState *playState;
};

