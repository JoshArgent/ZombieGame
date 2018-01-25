#include "DiedState.h"
#include "Psyja2Engine.h"
#include "PlayState.h"
#include "PlayerObject.h"
#include <sstream>

DiedState::DiedState(Psyja2Engine *engine, PlayState *playState) : State(engine), playState(playState)
{
	engine->SetupBackgroundBuffer();
	engine->Redraw(true);
	playState->getPlayer()->lives;
}

DiedState::~DiedState()
{
}

void DiedState::SetupBackgroundBuffer()
{
	playState->SetupBackgroundBuffer();
}

void DiedState::DrawStrings()
{
	playState->DrawStrings();
}

void DiedState::DrawStringsOnTop()
{
	// Shadow text:
	Font *font = engine->GetFont("Millimetre-Bold.otf", 30);

	std::ostringstream ss;
	ss << "You Died! Press Space To Respawn...";
	engine->DrawScreenString(engine->GetScreenWidth() / 2 - 230 + 2, engine->GetScreenHeight() / 2 + 2, ss.str().c_str(), 0x000000, font);
	engine->DrawScreenString(engine->GetScreenWidth() / 2 - 230, engine->GetScreenHeight() / 2, ss.str().c_str(), 0xFFFFFF, font);
}

void DiedState::GameAction()
{
	// If too early to act then do nothing
	if (!engine->IsTimeToActWithSleep())
		return;

	engine->SetTimeToAct(15);

	// Pause all object movements
	//engine->UpdateAllObjects(engine->GetTime());
}

void DiedState::KeyDown(int iKeyCode)
{
	if (iKeyCode == SDLK_SPACE)
	{
		playState->getPlayer()->SetVisible(true);
		engine->SwitchState(playState, false, true);
	}
}