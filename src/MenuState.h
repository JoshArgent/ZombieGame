#pragma once
#include "State.h"

class Button;

class MenuState : public State
{
public:
	MenuState(Psyja2Engine *engine);
	~MenuState();
	void SetupBackgroundBuffer();
	int InitialiseObjects();
	void MouseMoved(int iX, int iY);
	void MouseDown(int iButton, int iX, int iY);
	void DrawStrings();
	void GameAction();
private:
	Button *playButton;
	Button *helpButton;
};

