#pragma once

class Psyja2Engine;

class State
{
public:
	State(Psyja2Engine *engine);
	~State();
	virtual void SetupBackgroundBuffer();
	virtual int InitialiseObjects();
	virtual void MouseMoved(int iX, int iY);
	virtual void MouseDown(int iButton, int iX, int iY);
	virtual void DrawStrings();
	virtual void DrawStringsOnTop();
	virtual void GameAction();
	virtual void KeyDown(int iKeyCode);
	Psyja2Engine *engine;
};

