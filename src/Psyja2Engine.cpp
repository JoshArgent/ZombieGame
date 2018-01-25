#include "Psyja2Engine.h"
#include <sstream>
#include <fstream>
#include "PlayState.h"
#include "MenuState.h"
#include "header.h"
#include "SpriteManager.h"

Psyja2Engine::Psyja2Engine()
{
	// Load a list of exising map files
	for (int i = 0; true; i++)
	{
		std::ostringstream ss;
		ss << "maps/map" << std::to_string(i) << ".txt";
		std::ifstream infile(ss.str().c_str());
		if (!infile.good())
			break;
		levels.push_back(ss.str().c_str());
	}
	// Load sprites
	spriteManager = new SpriteManager();
	// Start the play state
	state = new MenuState(this);
}

Psyja2Engine::~Psyja2Engine()
{
	delete state;
	delete spriteManager;
}

void Psyja2Engine::SetupBackgroundBuffer()
{
	FillBackground(0xFFFFFF);
	state->SetupBackgroundBuffer();
}

int Psyja2Engine::InitialiseObjects()
{
	return state->InitialiseObjects();
}

void Psyja2Engine::MouseMoved(int iX, int iY)
{
	state->MouseMoved(iX, iY);
}

void Psyja2Engine::MouseDown(int iButton, int iX, int iY)
{
	state->MouseDown(iButton, iX, iY);
}

void Psyja2Engine::DrawStrings()
{
	state->DrawStrings();
}

void Psyja2Engine::DrawStringsOnTop()
{
	state->DrawStringsOnTop();
}

void Psyja2Engine::GameAction()
{
	state->GameAction();
}

void Psyja2Engine::KeyDown(int iKeyCode)
{
	state->KeyDown(iKeyCode);
}

void Psyja2Engine::SwitchState(State *state, bool keepState, bool keepObjects)
{	
	// Delete the old state and reinitialize the objects
	if (!keepState)
	{
		if (this->state != NULL)
			delete this->state;
	}
	this->state = state;
	if (!keepObjects)
	{
		InitialiseObjects();
	}		

	// Redraw the background and objects
	Redraw(true);
	SetupBackgroundBuffer();
}