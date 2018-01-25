#include "PauseState.h"
#include "PlayState.h"
#include "Psyja2Engine.h"

PauseState::PauseState(Psyja2Engine *engine, PlayState *playState) : State(engine), playState(playState)
{
	engine->SetupBackgroundBuffer();
	engine->Redraw(true);
}

PauseState::~PauseState()
{
	
}

void PauseState::SetupBackgroundBuffer()
{
	playState->SetupBackgroundBuffer();
}

void PauseState::DrawStrings()
{
	playState->DrawStrings();
}

void PauseState::DrawStringsOnTop()
{
	// Shadow text:
	Font *font = engine->GetFont("Millimetre-Bold.otf", 26);
	engine->DrawScreenString(engine->GetScreenWidth() / 2 - 50 + 2, engine->GetScreenHeight() / 2 + 2, "Game Paused!", 0x000000, font);
	engine->DrawScreenString(engine->GetScreenWidth() / 2 - 50, engine->GetScreenHeight() / 2, "Game Paused!", 0xFFFFFF, font);
}

void PauseState::GameAction()
{
	// If too early to act then do nothing
	if (!engine->IsTimeToActWithSleep())
		return;

	// Don't act for another 15 ticks
	engine->SetTimeToAct(15);

	// Pause all object movements
	//engine->UpdateAllObjects(engine->GetTime());
}

void PauseState::KeyDown(int iKeyCode)
{
	if (iKeyCode == SDLK_SPACE)
	{
		engine->SwitchState(playState, false, true);
	}
}
