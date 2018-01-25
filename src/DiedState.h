#pragma once
#include "State.h"

class PlayState;

class DiedState : public State
{
public:
	DiedState(Psyja2Engine *engine, PlayState *playState);
	~DiedState();
	void SetupBackgroundBuffer();
	void DrawStrings();
	void DrawStringsOnTop();
	void GameAction();
	void KeyDown(int iKeyCode);
private:
	PlayState *playState;
};

