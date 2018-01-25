#pragma once
#include "State.h"

class PlayState;

class PauseState : public State
{
public:
	PauseState(Psyja2Engine *engine, PlayState *playState);
	~PauseState();
	void SetupBackgroundBuffer();
	void DrawStrings();
	void DrawStringsOnTop();
	void GameAction();
	void KeyDown(int iKeyCode);
private:
	PlayState *playState;
};

