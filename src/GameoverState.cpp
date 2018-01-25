#include "GameoverState.h"
#include "Psyja2Engine.h"
#include "PlayState.h"
#include "PlayerObject.h"
#include "MenuState.h"
#include <sstream>

GameoverState::GameoverState(Psyja2Engine *engine, PlayState *playState) : State(engine), playState(playState)
{
	engine->SetupBackgroundBuffer();
	engine->Redraw(true);
}

GameoverState::~GameoverState()
{
}

void GameoverState::SetupBackgroundBuffer()
{
	playState->SetupBackgroundBuffer();
}

void GameoverState::DrawStrings()
{
	playState->DrawStrings();
}

void GameoverState::DrawStringsOnTop()
{
	// Shadow text:
	Font *font = engine->GetFont("Millimetre-Bold.otf", 35);
	engine->DrawScreenString(engine->GetScreenWidth() / 2 - 90 + 2, engine->GetScreenHeight() / 2 + 2, "Game Over", 0x000000, font);
	engine->DrawScreenString(engine->GetScreenWidth() / 2 - 90, engine->GetScreenHeight() / 2, "Game Over", 0xFFFFFF, font);
	font = engine->GetFont("Millimetre-Bold.otf", 26);
	std::ostringstream ss;
	ss << "Your Survived " << std::to_string(playState->round) << " Rounds With A Score Of " << std::to_string(playState->getPlayer()->score);
	engine->DrawScreenString(engine->GetScreenWidth() / 2 - 250 + 2, engine->GetScreenHeight() / 2 + 47, ss.str().c_str(), 0x000000, font);
	engine->DrawScreenString(engine->GetScreenWidth() / 2 - 250, engine->GetScreenHeight() / 2 + 45, ss.str().c_str(), 0xFFFFFF, font);
	font = engine->GetFont("Millimetre-Bold.otf", 18);
	engine->DrawScreenString(engine->GetScreenWidth() / 2 - 160 + 2, engine->GetScreenHeight() / 2 + 82, "(Press space to return to the menu)", 0x000000, font);
	engine->DrawScreenString(engine->GetScreenWidth() / 2 - 160, engine->GetScreenHeight() / 2 + 80, "(Press space to return to the menu)", 0xFFFFFF, font);
}

void GameoverState::GameAction()
{
	// If too early to act then do nothing
	if (!engine->IsTimeToActWithSleep())
		return;

	// Don't act for another 15 ticks
	engine->SetTimeToAct(15);

	// Pause all object movements
	//engine->UpdateAllObjects(engine->GetTime());
}

void GameoverState::KeyDown(int iKeyCode)
{
	if (iKeyCode == SDLK_SPACE)
	{
		delete playState;
		engine->ResetLevel();
		engine->SwitchState(new MenuState(engine), false, false);
	}
}