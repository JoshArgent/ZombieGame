#include "HelpState.h"
#include "Psyja2Engine.h"
#include "JPGImage.h"
#include "MenuState.h"
#include "SpriteManager.h"

HelpState::HelpState(Psyja2Engine *engine) : State(engine)
{
}

HelpState::~HelpState()
{
}

void HelpState::SetupBackgroundBuffer()
{
	engine->spriteManager->GetHelp()->RenderImage(engine->GetBackground(), 0, 0, 0, 0, engine->GetScreenWidth(), engine->GetScreenHeight());
}

int HelpState::InitialiseObjects()
{
	engine->DrawableObjectsChanged();
	engine->DestroyOldObjects();
	engine->CreateObjectArray(1);
	return 0;
}

void HelpState::DrawStrings()
{
	// Shadow text:
	Font *font = engine->GetFont("Millimetre-Bold.otf", 50);
	engine->DrawScreenString(engine->GetScreenWidth() / 2 - 140 + 5, 30 + 5, "Instructions", 0x000000, font);
	engine->DrawScreenString(engine->GetScreenWidth() / 2 - 140, 30, "Instructions", 0xFFFFFF, font);
}

void HelpState::KeyDown(int iKeyCode)
{
	if (iKeyCode == SDLK_ESCAPE)
	{
		// Exit the help menu
		MenuState *nextState = new MenuState(engine);
		engine->SwitchState(nextState, false, false);
	}
}