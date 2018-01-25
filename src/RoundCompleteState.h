#pragma once
#include "State.h"
#include <string>

class PlayState;

class RoundCompleteState : public State
{
public:
	RoundCompleteState(Psyja2Engine *engine, PlayState *oldState, PlayState *nextPlayState);
	~RoundCompleteState();
	void SetupBackgroundBuffer();
	void DrawStrings();
	void DrawStringsOnTop();
	void GameAction();
private:
	PlayState *nextPlayState;
	PlayState *oldState;
	std::string msg1;
	std::string msg2;
	int messageState = 0;
};

