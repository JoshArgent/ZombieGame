#pragma once
#include "DisplayableObject.h"

class Psyja2Engine;

class Button : public DisplayableObject
{
public:
	Button(Psyja2Engine *engine, char *text, int x, int y);
	~Button();
	void Draw();
	void DoUpdate(int iCurrentTime);
	bool IsPressed();
private:
	Psyja2Engine *engine;
	char *text;
	Font *font;
	bool mouseOver = false;
	int xOffset = 0;
	bool pressed = false;
};

