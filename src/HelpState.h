#pragma once
#include "State.h"
class HelpState :
	public State
{
public:
	HelpState(Psyja2Engine *engine);
	~HelpState();
	void SetupBackgroundBuffer();
	int InitialiseObjects();
	void DrawStrings();
	void KeyDown(int iKeyCode);
};

